// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm hypervisor project and contributors

//! NullVM hypervisor management service entry point.

pub(crate) use nullvm_common::log;
pub mod cpu;

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
}
