// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm hypervisor project and contributors

//! Kernel Virtual Machine (KVM) related declarations.

use crate::vmm::{VmmResult, create_owned_fd};
use libc::{O_CLOEXEC, O_RDWR};
use std::os::fd::OwnedFd;

/// KVM subsystem handle struct.
pub struct Kvm {
    /// KVM file descriptor.
    fd: OwnedFd,
}

impl Kvm {
    /// Construct new KVM handle.
    ///
    /// # Returns
    /// - New `Kvm` object - in case of success.
    /// - `Err` - otherwise.
    pub fn new() -> VmmResult<Self> {
        // O_RDWR - open for read & write (used for setting VM).
        // O_CLOEXEC - close file descriptors in child processes
        // after a fork or exec (used for security, process isolation &
        // preventing resource leaks).
        let fd = create_owned_fd("/dev/kvm", O_RDWR | O_CLOEXEC)?;
        let kvm = Self { fd };

        Ok(kvm)
    }
}
