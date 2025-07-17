// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm hypervisor project and contributors

//! Virtual machine related declarations.

use crate::vmm::{VmmError, VmmResult};
use kvm_ioctls::Kvm;

/// Virtual machine info struct.
pub struct VirtualMachine {
    /// KVM subsystem handle.
    kvm: Kvm,
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
            return Err(VmmError::from("Incorrect KVM API version".to_string()));
        }

        let result = Self { kvm };
        Ok(result)
    }
}
