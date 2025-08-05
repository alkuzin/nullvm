// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// KVM virtual CPU file descriptor handle.

#include <nullvm/core/vcpu.hpp>
#include <linux/kvm.h>
#include <sys/mman.h>
#include <unistd.h>

namespace nullvm::core {

    auto VCpu::init(i32 raw, usize size) noexcept -> VmmResult<None> {

        if (raw < 0) {
            const auto err = "Invalid file descriptor: must be non-negative";
            return std::unexpected(err);
        }

        if (raw == 0 || raw == 1 || raw == 2) {
            const auto err = "Invalid file descriptor: cannot be 0, 1, or 2 "
            "(stdin, stdout, stderr)";
            return std::unexpected(err);
        }

        if (size == 0) {
            return std::unexpected(
                "Error to init virtual CPU: mapping memory size is zero"
            );
        }

        const auto prot = PROT_READ | PROT_WRITE;
        const auto addr = mmap(nullptr, size, prot, MAP_SHARED, raw, 0);

        if(auto result = this->state.init(addr, size); !result) {
            return std::unexpected(result.error());
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
