// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// KVM virtual machine file descriptor handle.

#include <nullvm/core/vmfd.hpp>
#include <linux/kvm.h>
#include <sys/ioctl.h>

namespace nullvm::core {

    auto VmFd::init(i32 fd) noexcept -> VmmResult<None> {

        if (fd < 0) {
            return std::unexpected(
            "Invalid file descriptor: must be non-negative"
            );
        }

        if (fd == 0 || fd == 1 || fd == 2) {
            return std::unexpected(
                "Invalid file descriptor: cannot be 0, 1, or 2 "
                "(stdin, stdout, stderr)"
            );
        }

        m_fd = FDWrapper(fd);
        return None {};
    }

    auto VmFd::fd() const noexcept -> i32 {
        return m_fd.fd();
    }

    auto VmFd::create_vcpu() const -> VmmResult<i32> {

        const auto result = ioctl(m_fd.fd(), KVM_CREATE_VCPU, 0);

        if (result == -1)
            return std::unexpected("Error to create virtual CPU");

        return result;
    }

}