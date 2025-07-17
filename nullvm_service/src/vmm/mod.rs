// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm hypervisor project and contributors

//! Virtual machine monitor main module.

use std::{
    fmt::{Debug, Display, Formatter},
    {error::Error, io},
};

pub mod vm;

use kvm_ioctls::Error as KvmError;

/// VMM error enumeration.
#[derive(Debug)]
pub enum VmmError {
    /// Custom error message.
    Custom(String),
    /// An I/O error.
    Io(io::Error),
    /// KVM specific error.
    Kvm(KvmError),
}

/// Format VMM error.
impl Display for VmmError {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        match self {
            VmmError::Custom(msg) => write!(f, "<custom>: {msg}"),
            VmmError::Io(err) => write!(f, "<I/O>: {err}"),
            VmmError::Kvm(err) => write!(f, "<KVM>: {err}"),
        }
    }
}

impl Error for VmmError {}

/// Convert `String` to VMM error.
impl From<String> for VmmError {
    fn from(value: String) -> Self {
        Self::Custom(value)
    }
}

/// Convert I/O error to VMM error.
impl From<io::Error> for VmmError {
    fn from(value: io::Error) -> Self {
        Self::Io(value)
    }
}

/// Convert KVM error to VMM error.
impl From<KvmError> for VmmError {
    fn from(value: KvmError) -> Self {
        Self::Kvm(value)
    }
}

/// VMM Result type wrapper.
pub type VmmResult<T> = Result<T, VmmError>;

/// Initialize and run virtual machine monitor.
///
/// # Returns
/// - `Ok`  - in case of success.
/// - `Err` - otherwise.
pub fn run() -> VmmResult<()> {
    Ok(())
}
