// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// Kernel-based Virtual Machine (KVM) subsystem handle.

#ifndef NULLVM_CORE_KVM_HPP
#define NULLVM_CORE_KVM_HPP

#include "nullvm/types.hpp"
#include <nullvm/core/vmfd.hpp>

namespace nullvm::core {

    /// KVM subsystem handler struct.
    class Kvm final {
        /// KVM file descriptor.
        FDWrapper m_fd;

    public:
        /// @brief Initialize Kvm object.
        ///
        /// @return None - in case of success.
        /// @return VmmError - otherwise.
        auto init() noexcept -> VmmResult<None>;

        /// @brief Get raw file descriptor value.
        ///
        /// @return Raw file descriptor value.
        auto fd() const noexcept -> i32;

        /// @brief Get virtual CPU memory map size.
        ///
        /// @return Virtual CPU memory map size in bytes - in case of success.
        /// @return VmmError - otherwise.
        auto vcpu_mmap_size() -> VmmResult<usize>;

        /// @brief Create virtual machine.
        ///
        /// @return New virtual machine file descriptor - in case of success.
        /// @return VmmError - otherwise.
        auto create_vm() const -> VmmResult<i32>;
    };

}

#endif // NULLVM_CORE_KVM_HPP
