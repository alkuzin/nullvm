// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// KVM virtual machine file descriptor handle.

#ifndef NULLVM_CORE_VMFD_HPP
#define NULLVM_CORE_VMFD_HPP

#include <nullvm/types.hpp>

namespace nullvm::core {

    /// Virtual machine file descriptor management struct.
    struct VmFd {
        /// Raw virtual machine file descriptor.
        i32 raw;

        /// @brief Construct new VmFd object.
        VmFd() noexcept: raw(-1) {}

        /// @brief Destruct VmFd object.
        ~VmFd() noexcept;

        /// @brief Initialize VmFd object.
        ///
        /// @param [in] raw given raw virtual machine file descriptor.
        ///
        /// @return None - in case of success.
        /// @return VmmError - otherwise.
        auto init(i32 raw) noexcept -> VmmResult<None>;
    };

}

#endif // NULLVM_CORE_VMFD_HPP
