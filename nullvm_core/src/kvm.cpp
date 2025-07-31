// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// Kernel-based Virtual Machine (KVM) subsystem handle.

#include <nullvm/core/kvm.hpp>
#include <nullvm/log.hpp>
#include <linux/kvm.h>
#include <sys/ioctl.h>
#include <fcntl.h>

namespace nullvm::core {

    // Special file that provides an interface to the KVM subsystem.
    constexpr auto KVM_FILE = "/dev/kvm";

    auto Kvm::init() noexcept -> VmmResult<None> {
        // O_RDWR - read & write permission flag.
        // O_CLOEXEC - ensures that the file descriptor is automatically closed
        // during the execution of an exec() family function.
        this->raw = open("/dev/kvm", O_RDWR | O_CLOEXEC);

        if (this->raw == -1) {
            const auto err = std::format("Error to open '{}'", KVM_FILE);
            return std::unexpected(err);
        }

        // Check KVM version.
        const auto ret = ioctl(this->raw, KVM_GET_API_VERSION, 0);

        if (ret == -1)
            return std::unexpected("Error to get KVM API version");

        if (ret != KVM_API_VERSION) {
            const auto err = std::format(
                "Expected KVM API version {}, but got {}",
                KVM_API_VERSION, ret
            );
            return std::unexpected(err);
        }

        return None {};
    }

    Kvm::~Kvm() noexcept {
        if (this->raw != -1) {
            close(this->raw);
            this->raw = -1;
        }
    }

    auto Kvm::create_vm() const -> VmmResult<i32> {
        const auto vmfd = ioctl(this->raw, KVM_CREATE_VM, 0);

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