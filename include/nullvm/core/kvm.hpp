// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// Kernel-based Virtual Machine (KVM) subsystem handle.

#ifndef NULLVM_CORE_KVM_HPP
#define NULLVM_CORE_KVM_HPP

#include <nullvm/core/vmfd.hpp>

namespace nullvm::core {

    /// KVM subsystem handler struct.
    struct Kvm {
        /// Raw KVM file descriptor.
        i32 raw;

        /// @brief Construct new Kvm object.
        Kvm() noexcept: raw(-1) {}

        /// @brief Destruct Kvm object.
        ~Kvm() noexcept;

        /// @brief Initialize Kvm object.
        ///
        /// @return None - in case of success.
        /// @return VmmError - otherwise.
        auto init() noexcept -> VmmResult<None>;

        /// @brief Create virtual machine.
        ///
        /// @return New virtual machine file descriptor - in case of success.
        /// @return VmmError - otherwise.
        auto create_vm() const -> VmmResult<i32>;
    };

}

#endif // NULLVM_CORE_KVM_HPP
