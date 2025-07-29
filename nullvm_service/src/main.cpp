// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// NullVM service entry point.

#include <nullvm/types.hpp>
#include <nullvm/log.hpp>
#include <print>

using namespace nullvm;

auto main() -> i32 {
    log::info("Running NullVM service");
    return 0;
}
