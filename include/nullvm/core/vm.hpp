// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// Virtual machine related declarations.

#ifndef NULLVM_CORE_VM_HPP
#define NULLVM_CORE_VM_HPP

#include <nullvm/core/kvm.hpp>

namespace nullvm::core {

    /// Virtual machine info struct.
    struct VirtualMachine {
        /// KVM subsystem handle.
        Kvm kvm;
        /// Virtual machine file descriptor.
        VmFd vmfd;

        /// @brief Construct new VirtualMachine object.
        VirtualMachine() noexcept = default;

        /// @brief Initialize VirtualMachine object.
        ///
        /// @return None - in case of success.
        /// @return VmmError - otherwise.
        auto init() noexcept -> VmmResult<None>;
    };

}

#endif // NULLVM_CORE_VM_HPP
