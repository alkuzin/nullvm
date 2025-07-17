// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm hypervisor project and contributors

//! Virtual machine monitor main module.

use libc::c_int;
use std::{
    error::Error,
    ffi::CString,
    io,
    os::fd::{FromRawFd, OwnedFd},
};

pub mod kvm;
mod vcpu;
pub mod vm;

/// VMM Result type wrapper.
pub type VmmResult<T> = Result<T, Box<dyn Error>>;

/// Create owned file descriptor from path.
///
/// # Parameters
/// - `path`  - given file path to open.
/// - `flags` - given file flags.
///
/// # Returns
/// - `Owned file descriptor` - in case of success.
/// - `Err` - otherwise.
pub fn create_owned_fd(path: &str, flags: c_int) -> VmmResult<OwnedFd> {
    let c_path =
        CString::new(path).map_err(|e| Box::new(e) as Box<dyn Error>)?;

    let fd = unsafe { libc::open(c_path.as_ptr(), flags) };

    if fd < 0 {
        return Err(Box::new(io::Error::last_os_error()));
    }

    let owned_fd = unsafe { OwnedFd::from_raw_fd(fd) };
    Ok(owned_fd)
}

/// Initialize and run virtual machine monitor.
///
/// # Returns
/// - `Ok`  - in case of success.
/// - `Err` - otherwise.
pub fn run() -> VmmResult<()> {
    Ok(())
}
