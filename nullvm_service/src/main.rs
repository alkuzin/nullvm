// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm hypervisor project and contributors

//! NullVM hypervisor management service entry point.

pub mod cpu;
pub mod vmm;

pub use nullvm_common::*;

fn main() {
    log::info!("Running NullVM hypervisor management service");
    log::info!("Detected CPU:");
    log::info!("CPU vendor: {}", cpu::get_cpu_vendor());
    log::info!("CPU brand: {}", cpu::get_cpuid_brand());

    if !cpu::is_virtualization_supported() {
        log::error!("This CPU doesn't support virtualization");
        std::process::exit(1);
    }

    log::info!("This CPU support virtualization");

    if let Err(error) = vmm::run() {
        log::error!("Error: {error}")
    }
}
