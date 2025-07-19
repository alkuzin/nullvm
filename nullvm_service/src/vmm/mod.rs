// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm hypervisor project and contributors

//! Virtual machine monitor main module.

pub mod vm;

use crate::{
    VmmResult, cpu,
    log::{self, *},
};

/// Initialize and run virtual machine monitor.
///
/// # Returns
/// - `Ok`  - in case of success.
/// - `Err` - otherwise.
pub fn run() -> VmmResult<()> {
    // TODO: add config.
    log::logger!("nullvm_service.log");

    log::info!("Running NullVM hypervisor management service");
    log::info!("Detected CPU:");
    log::info!("CPU vendor: {}", cpu::get_cpu_vendor());
    log::info!("CPU brand: {}", cpu::get_cpuid_brand());

    if !cpu::is_virtualization_supported() {
        log::error!("This CPU doesn't support virtualization");
        std::process::exit(1);
    }

    log::info!("This CPU support virtualization");

    Ok(())
}
