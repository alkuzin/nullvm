// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// KVM virtual CPU file descriptor handle.

#include <nullvm/core/vcpu.hpp>
#include <unistd.h>

namespace nullvm::core {

    auto VCpu::init(i32 raw) noexcept -> VmmResult<None> {
        if (raw < 0) {
            const auto err = "Invalid file descriptor: must be non-negative";
            return std::unexpected(err);
        }

        if (raw == 0 || raw == 1 || raw == 2) {
            const auto err = "Invalid file descriptor: cannot be 0, 1, or 2 "
            "(stdin, stdout, stderr)";
            return std::unexpected(err);
        }

        this->raw = raw;
        return None {};
    }

    VCpu::~VCpu() noexcept {
        if (this->raw != -1) {
            close(this->raw);
            this->raw = -1;
        }
    }

}
