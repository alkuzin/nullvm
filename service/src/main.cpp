// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// NullVM service entry point.

#include <nullvm/core/cpu.hpp>
#include <nullvm/log.hpp>

using namespace nullvm;

auto main() -> i32 {
    log::info("Running NullVM hypervisor management service");
    log::info("Detected CPU:");
    log::info("CPU vendor: {}", core::get_cpu_vendor());
    log::info("CPU brand: {}", core::get_cpu_brand());

    if (!core::is_virtualization_supported()) {
        log::error("This CPU doesn't support virtualization");
        std::exit(EXIT_FAILURE);
    }

    log::info("This CPU support virtualization");
    return 0;
}
