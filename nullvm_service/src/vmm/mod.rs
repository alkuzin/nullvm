// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm hypervisor project and contributors

//! Virtual machine monitor main module.

use crate::VmmResult;

pub mod vm;

/// Initialize and run virtual machine monitor.
///
/// # Returns
/// - `Ok`  - in case of success.
/// - `Err` - otherwise.
pub fn run() -> VmmResult<()> {
    Ok(())
}
