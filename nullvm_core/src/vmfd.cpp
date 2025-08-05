// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// KVM virtual machine file descriptor handle.

#include <nullvm/core/vmfd.hpp>
#include <linux/kvm.h>
#include <sys/ioctl.h>
#include <unistd.h>

namespace nullvm::core {

    auto VmFd::init(const i32 raw) noexcept -> VmmResult<None> {
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

    VmFd::~VmFd() noexcept {
        if (this->raw != -1) {
            close(this->raw);
            this->raw = -1;
        }
    }

    auto VmFd::create_vcpu() const -> VmmResult<i32> {
        const auto result = ioctl(this->raw, KVM_CREATE_VCPU, 0);

        if (result == -1)
            return std::unexpected("Error to create virtual CPU");

        return result;
    }

}