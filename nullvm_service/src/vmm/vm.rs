// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm hypervisor project and contributors

//! Virtual machine related declarations.

use crate::vmm::{VmmError, VmmResult};
use kvm_bindings::kvm_userspace_memory_region;
use kvm_ioctls::{Kvm, VcpuFd, VmFd};
use libc::{MAP_ANONYMOUS, MAP_SHARED, PROT_READ, PROT_WRITE, mmap};
use std::ptr::null_mut;

/// Virtual machine info struct.
pub struct VirtualMachine {
    /// KVM subsystem handle.
    kvm: Kvm,
    /// Virtual machine file descriptor.
    vmfd: VmFd,
    /// Virtual CPU file descriptor.
    vcpu: VcpuFd,
    /// Memory region of VM.
    mem_region: kvm_userspace_memory_region,
}

impl VirtualMachine {
    /// Construct new virtual machine.
    ///
    /// # Returns
    /// - New `VirtualMachine` object.
    pub fn new() -> VmmResult<Self> {
        let kvm = Kvm::new()?;
        let version = kvm.get_api_version();

        if version < 0 {
            return Err(VmmError::from(
                "Incorrect KVM API version".to_string(),
            ));
        }

        let vmfd = kvm.create_vm()?;
        let vcpu = vmfd.create_vcpu(0)?;

        let result = Self {
            kvm,
            vmfd,
            vcpu,
            mem_region: Default::default(),
        };
        Ok(result)
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
        if mem_size == 0 {
            let msg = "Mapping memory size is zero".to_string();
            return Err(VmmError::from(msg));
        }

        let userspace_addr = unsafe {
            // Mapping protection flags.
            let prot = PROT_READ | PROT_WRITE;

            // MAP_SHARED flag ensures that both the host and guest can see the
            // same changes to the memory.
            // MAP_ANONYMOUS flag indicates that the mapping is not
            // backed by any file.
            let flags = MAP_SHARED | MAP_ANONYMOUS;

            mmap(null_mut(), mem_size, prot, flags, -1, 0)
        };

        let mem_region = kvm_userspace_memory_region {
            // Memory region's unique slot number.
            slot: 0,
            // Flags that modify the behavior of the memory region.
            flags: 0,
            guest_phys_addr: guest_paddr,
            memory_size: mem_size as u64,
            userspace_addr: userspace_addr as u64,
        };

        unsafe {
            self.vmfd.set_user_memory_region(mem_region)?;
        }

        self.mem_region = mem_region;
        Ok(())
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
        let result = vm.set_user_mem_region(0, 0x1000);

        assert!(result.is_ok())
    }

    #[test]
    fn test_set_user_mem_region_zero_mem_size() {
        let mut vm = VirtualMachine::new().unwrap();
        let result = vm.set_user_mem_region(0, 0);

        assert!(result.is_err())
    }
}
