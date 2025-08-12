// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// Kernel-based Virtual Machine (KVM) subsystem handle.

#include "nullvm/types.hpp"
#include <expected>
#include <nullvm/core/kvm.hpp>
#include <nullvm/log.hpp>
#include <linux/kvm.h>
#include <sys/ioctl.h>
#include <fcntl.h>

namespace nullvm::core {

    namespace {
        // Special file that provides an interface to the KVM subsystem.
        constexpr auto KVM_FILE = "/dev/kvm";
    }

    auto Kvm::init() noexcept -> VmmResult<None> {
        // O_RDWR - read & write permission flag.
        // O_CLOEXEC - ensures that the file descriptor is automatically closed
        // during the execution of an exec() family function.
        const auto fd = open("/dev/kvm", O_RDWR | O_CLOEXEC);

        if (fd == -1) {
            const auto err = std::format("Error to open '{}'", KVM_FILE);
            return std::unexpected(err);
        }

        // Check KVM version.
        const auto ret = ioctl(fd, KVM_GET_API_VERSION, 0);

        if (ret == -1)
            return std::unexpected("Error to get KVM API version");

        if (ret != KVM_API_VERSION) {
            const auto err = std::format(
                "Expected KVM API version {}, but got {}",
                KVM_API_VERSION, ret
            );
            return std::unexpected(err);
        }

        m_fd = FDWrapper(fd);
        return None {};
    }

    auto Kvm::fd() const noexcept -> i32 {
        return m_fd.fd();
    }

    auto Kvm::vcpu_mmap_size() -> VmmResult<usize> {
        const auto ret = ioctl(m_fd.fd(), KVM_GET_VCPU_MMAP_SIZE, 0);

        if (ret == -1)
            return std::unexpected("Error to get vCPU mmap size");

        return static_cast<usize>(ret);
    }

    auto Kvm::create_vm() const -> VmmResult<i32> {

        const auto vmfd = ioctl(m_fd.fd(), KVM_CREATE_VM, 0);

        if (vmfd == -1) {
            const auto err = std::format(
                "Error to create VM file descriptor: {}",
                std::strerror(errno)
            );
            return std::unexpected(err);
        }

        return vmfd;
    }

}