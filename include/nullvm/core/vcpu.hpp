// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// KVM virtual CPU file descriptor handle.

#ifndef NULLVM_CORE_VCPU_HPP
#define NULLVM_CORE_VCPU_HPP

#include <nullvm/types.hpp>

namespace nullvm::core {

    /// Virtual CPU file descriptor management struct.
    struct VCpu {
        /// Raw virtual CPU file descriptor.
        i32 raw;

        /// @brief Construct new VCpu object.
        VCpu() noexcept: raw(-1) {}

        /// @brief Destruct VCpu object.
        ~VCpu() noexcept;

        /// @brief Initialize VCpu object.
        ///
        /// @param [in] raw given raw virtual CPU file descriptor.
        ///
        /// @return None - in case of success.
        /// @return VmmError - otherwise.
        auto init(i32 raw) noexcept -> VmmResult<None>;
    };

}

#endif // NULLVM_CORE_VCPU_HPP
