// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm hypervisor project and contributors

//! Virtual machine related declarations.

use crate::{VmmError, VmmResult};
use kvm_bindings::kvm_userspace_memory_region;
use kvm_ioctls::{Kvm, VcpuExit, VcpuFd, VmFd};
use libc::{
    MAP_ANONYMOUS, MAP_FAILED, MAP_SHARED, PROT_READ, PROT_WRITE, mmap, munmap,
};
use nullvm_common::{MMapWrapper, log};
use std::{
    io::Write,
    ops::{Deref, DerefMut},
    ptr,
    ptr::null_mut,
};

/// Virtual machine info struct.
pub struct VirtualMachine {
    /// KVM subsystem handle.
    kvm: Kvm,
    /// Virtual machine file descriptor.
    vmfd: VmFd,
    /// Virtual CPU file descriptor.
    vcpu: VcpuFd,
    /// Memory allocated to VM.
    memory: Option<MMapWrapper>,
    /// VM's memory region info.
    mem_region: kvm_userspace_memory_region,
}

impl VirtualMachine {
    /// Construct new virtual machine.
    ///
    /// # Returns
    /// - New `VirtualMachine` object - in case of success.
    /// - `Err` - otherwise.
    pub fn new() -> VmmResult<Self> {
        let kvm = Kvm::new()?;
        let version = kvm.get_api_version();

        if version < 0 {
            return Err(VmmError::from("Incorrect KVM API version"));
        }

        let vmfd = kvm.create_vm()?;
        let vcpu = vmfd.create_vcpu(0)?;

        let result = Self {
            kvm,
            vmfd,
            vcpu,
            memory: None,
            mem_region: Default::default(),
        };

        Ok(result)
    }

    /// Set VM's memory.
    ///
    /// # Parameters
    /// - `memory_size` - given size of the memory region in bytes to allocate.
    ///
    /// # Returns
    /// - `Ok`  - in case of success.
    /// - `Err` - otherwise.
    fn set_vm_memory(&mut self, mem_size: usize) -> VmmResult<()> {
        if mem_size == 0 {
            return Err(VmmError::from("Mapping memory size is zero"));
        }

        let addr = unsafe {
            // Mapping protection flags.
            let prot = PROT_READ | PROT_WRITE;

            // MAP_SHARED flag ensures that both the host and guest can see the
            // same changes to the memory.
            // MAP_ANONYMOUS flag indicates that the mapping is not
            // backed by any file.
            let flags = MAP_SHARED | MAP_ANONYMOUS;

            let addr = mmap(null_mut(), mem_size, prot, flags, -1, 0);

            if addr == MAP_FAILED {
                return Err(VmmError::from("Failed to map memory"));
            }

            addr
        };

        let cleanup = Box::new(move |value| unsafe {
            munmap(value, mem_size);
        });

        self.memory = Some(MMapWrapper::new(addr, cleanup));
        Ok(())
    }

    /// Set userspace memory region.
    ///
    /// # Parameters
    /// - `guest_paddr` - given starting address in the guest's address space.
    /// - `memory_size` - given size of the memory region in bytes.
    ///
    /// # Returns
    /// - `Ok`  - in case of success.
    /// - `Err` - otherwise.
    pub fn set_user_mem_region(
        &mut self,
        guest_paddr: u64,
        mem_size: usize,
    ) -> VmmResult<()> {
        self.set_vm_memory(mem_size)?;

        if let Some(memory) = &self.memory {
            let mem_region = kvm_userspace_memory_region {
                // Memory region's unique slot number.
                slot: 0,
                // Flags that modify the behavior of the memory region.
                flags: 0,
                guest_phys_addr: guest_paddr,
                memory_size: mem_size as u64,
                userspace_addr: *memory.deref() as u64,
            };

            unsafe {
                self.vmfd.set_user_memory_region(mem_region)?;
            }

            self.mem_region = mem_region;

            // Set up the initial state of special registers.
            let mut sregs = self.vcpu.get_sregs()?;
            sregs.cs.base = 0;
            sregs.cs.selector = 0;
            self.vcpu.set_sregs(&sregs)?;

            // Set up the initial state of standard registers.
            let mut regs = self.vcpu.get_regs()?;
            regs.rip = guest_paddr;
            // Rflags bit 1 should always be set.
            regs.rflags = 0x2;
            self.vcpu.set_regs(&regs)?;

            return Ok(());
        }

        Err(VmmError::from("VM's memory wasn't set"))
    }

    /// Load raw binary contents to VM's memory.
    ///
    /// # Parameters
    /// - `raw` - given raw binary bytes to load.
    ///
    /// # Returns
    /// - `Ok`  - in case of success.
    /// - `Err` - otherwise.
    pub fn load_raw(&mut self, raw: &[u8]) -> VmmResult<()> {
        if let Some(memory) = self.memory.as_mut() {
            let addr = memory.deref_mut();

            unsafe {
                ptr::copy_nonoverlapping(raw.as_ptr(), addr.cast(), raw.len());
            }

            return Ok(());
        }

        Err(VmmError::from("Error to load raw binary"))
    }

    /// Run virtual machine.
    ///
    /// # Returns
    /// - `Ok`  - in case of success.
    /// - `Err` - otherwise.
    pub fn run(&mut self) -> VmmResult<()> {
        loop {
            let reason = self.vcpu.run()?;
            log::debug!("Reason: {reason:?}");

            match reason {
                VcpuExit::IoOut(port, data) => Self::handle_io_out(port, data)?,
                VcpuExit::Hlt => return Self::handle_hlt(),
                VcpuExit::InternalError => Self::handle_internal_error()?,
                _ => Self::unhandled_exit(reason)?,
            }
        }
    }

    /// Handle data being sent to a I/O port.
    ///
    /// # Parameters
    /// - `port` - given I/O port.
    /// - `data` - given data to send.
    ///
    /// # Returns
    /// - `Ok`  - in case of success.
    /// - `Err` - otherwise.
    fn handle_io_out(port: u16, data: &[u8]) -> VmmResult<()> {
        log::debug!("Out from port {port:#05X} to data {data:?}");

        // TODO: check in settings where to write
        // (stdout or specified file).
        if port == 0x3f8 {
            std::io::stdout().write_all(data)?;
            std::io::stdout().flush()?;
        }
        else {
            return Err(VmmError::from("Unhandled I/O out port"));
        }

        Ok(())
    }

    /// Handle CPU halt.
    ///
    /// # Returns
    /// - `Ok`  - in case of success.
    /// - `Err` - otherwise.
    fn handle_hlt() -> VmmResult<()> {
        log::debug!("Halt CPU");
        Ok(())
    }

    /// Handle internal error.
    ///
    /// # Returns
    /// - `Ok`  - in case of success.
    /// - `Err` - otherwise.
    fn handle_internal_error() -> VmmResult<()> {
        Err(VmmError::from("Internal error occurred"))
    }

    /// Handle unhandled VM exit reason.
    ///
    /// # Parameters
    /// - `reason` - given unhandled VM exit reason.
    ///
    /// # Returns
    /// - `Ok`  - in case of success.
    /// - `Err` - otherwise.
    fn unhandled_exit(reason: VcpuExit) -> VmmResult<()> {
        let error = format!("Unhandled exit reason: {reason:#?}");
        Err(VmmError::from(error))
    }
}

#[cfg(test)]
pub mod tests {
    use crate::log::*;
    use super::*;

    fn setup() {
        log::logger!("test_vm.log");
    }

    #[test]
    fn test_create_vm() {
        setup();

        let vm = VirtualMachine::new();
        assert!(vm.is_ok());
    }

    #[test]
    fn test_set_user_mem_region() {
        setup();

        let mut vm = VirtualMachine::new().unwrap();
        let result = vm.set_user_mem_region(0x1000, 0x1000);

        assert!(result.is_ok())
    }

    #[test]
    fn test_set_user_mem_region_zero_mem_size() {
        setup();

        let mut vm = VirtualMachine::new().unwrap();
        let result = vm.set_user_mem_region(0, 0);

        assert!(result.is_err())
    }

    #[test]
    fn test_run_code1() {
        setup();

        // mov $0x3f8, %dx
        // add %bl, %al
        // add $'0', %al
        // out %al, (%dx)
        // mov $'\n', %al
        // out %al, (%dx)
        // hlt
        const CODE: [u8; 12] = [
            0xba, 0xf8, 0x03, 0x00, 0xd8, 0x04, b'0', 0xee, 0xb0, b'\n',
            0xee, 0xf4
        ];

        let mut vm = VirtualMachine::new().unwrap();
        let _ = vm.set_user_mem_region(0x1000, 0x1000).unwrap();

        // Setting al, bl registers before testing code.
        let mut regs = vm.vcpu.get_regs().unwrap();
        regs.rax = 4;
        regs.rbx = 2;
        vm.vcpu.set_regs(&regs).unwrap();

        let result = vm.load_raw(&CODE);
        assert!(result.is_ok());

        let result = vm.run();
        assert!(result.is_ok());
    }

    #[test]
    fn test_run_code2() {
        setup();

        // mov ax,0x42
        // mov [ds:0x1000],ax
        // hlt
        const CODE: [u8; 8] = [0xb8, 0x42, 0x00, 0x3e, 0xa3, 0x00, 0x10, 0xf4];

        let mut vm = VirtualMachine::new().unwrap();
        let _ = vm.set_user_mem_region(0x0, 0x2000).unwrap();

        let result = vm.load_raw(&CODE);
        assert!(result.is_ok());

        let result = vm.run();
        assert!(result.is_ok());

        let addr = vm.mem_region.userspace_addr;
        let size = vm.mem_region.memory_size;
        let mem_slice = unsafe {
            std::slice::from_raw_parts(addr as * const u8, size as usize)
        };

        for (i, val) in mem_slice.iter().enumerate() {
            if *val != 0 {
                log::test!("({i}) val: {val:#04X}");
            }
        }

        assert_eq!(mem_slice[0x1000] as i32, 0x42);
    }
}
