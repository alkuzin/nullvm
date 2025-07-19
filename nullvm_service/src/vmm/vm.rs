// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm hypervisor project and contributors

//! Virtual machine related declarations.

use crate::{VmmError, VmmResult};
use kvm_bindings::{kvm_run, kvm_userspace_memory_region};
use kvm_ioctls::{Kvm, VcpuFd, VmFd};
use libc::{
    MAP_ANONYMOUS, MAP_FAILED, MAP_SHARED, PROT_READ, PROT_WRITE, mmap, munmap,
};
use nullvm_common::MMapWrapper;
use std::{ops::{Deref, DerefMut}, ptr, ptr::null_mut};

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
    /// Virtual CPU state.
    kvm_run: Option<MMapWrapper>,
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
            return Err(VmmError::from(
                "Incorrect KVM API version".to_string(),
            ));
        }

        let vmfd = kvm.create_vm()?;
        let mut vcpu = vmfd.create_vcpu(0)?;

        let kvm_run = vcpu.get_kvm_run();

        let result = Self {
            kvm,
            vmfd,
            vcpu,
            memory: None,
            mem_region: Default::default(),
            kvm_run: None,
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
            let msg = "Mapping memory size is zero".to_string();
            return Err(VmmError::from(msg));
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
                return Err(VmmError::from("Failed to map memory".to_string()));
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
            return Ok(());
        }

        Err(VmmError::from("VM's memory wasn't set".to_string()))
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

        Err(VmmError::from("Error to load raw binary".to_string()))
    }
}

#[cfg(test)]
pub mod tests {
    use super::*;

    #[test]
    fn test_create_vm() {
        let vm = VirtualMachine::new();
        assert!(vm.is_ok());
    }

    #[test]
    fn test_set_user_mem_region() {
        let mut vm = VirtualMachine::new().unwrap();
        let result = vm.set_user_mem_region(0x1000, 0x1000);

        assert!(result.is_ok())
    }

    #[test]
    fn test_set_user_mem_region_zero_mem_size() {
        let mut vm = VirtualMachine::new().unwrap();
        let result = vm.set_user_mem_region(0, 0);

        assert!(result.is_err())
    }

    #[test]
    fn test_run_code() {
        const CODE: [u8;12] = [
            0xba, 0xf8, 0x03, // mov $0x3f8, %dx
            0x00, 0xd8,       // add %bl, %al
            0x04, b'0',       // add $'0', %al
            0xee,             // out %al, (%dx)
            0xb0, b'\n',      // mov $'\n', %al
            0xee,             // out %al, (%dx)
            0xf4,             // hlt
        ];

        let mut vm = VirtualMachine::new().unwrap();
        let _ = vm.set_user_mem_region(0x1000, 0x1000).unwrap();

        let result = vm.load_raw(&CODE);
        assert!(result.is_ok());

        // TODO: run VM.
    }
}
