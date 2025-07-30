// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// KVM virtual machine file descriptor handle.

#include <nullvm/core/vmfd.hpp>
#include <unistd.h>

namespace nullvm::core {

    auto VmFd::create(const i32 raw) noexcept -> VmmResult<VmFd> {
        if (raw < 0) {
            const auto err = "Invalid file descriptor: must be non-negative";
            return std::unexpected(err);
        }

        if (raw == 0 || raw == 1 || raw == 2) {
            const auto err = "Invalid file descriptor: cannot be 0, 1, or 2 "
            "(stdin, stdout, stderr)";
            return std::unexpected(err);
        }

        return VmFd { .raw = raw };
    }

    VmFd::~VmFd() noexcept {
        close(this->raw);
        this->raw = -1;
    }

}