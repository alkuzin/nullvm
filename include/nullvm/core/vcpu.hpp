// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// KVM virtual CPU file descriptor handle.

#ifndef NULLVM_CORE_VCPU_HPP
#define NULLVM_CORE_VCPU_HPP

#include <nullvm/core/mmap_wrapper.hpp>
#include <nullvm/types.hpp>
#include <linux/kvm.h>

namespace nullvm::core {

    /// Virtual CPU file descriptor management struct.
    struct VCpu {
        /// Raw virtual CPU file descriptor.
        i32 raw;
        /// Virtual CPU state.
        MMapWrapper state;

        /// @brief Construct new VCpu object.
        VCpu() noexcept: raw(-1) {}

        /// @brief Destruct VCpu object.
        ~VCpu() noexcept;

        /// @brief Initialize VCpu object.
        ///
        /// @param [in] raw given raw virtual CPU file descriptor.
        /// @param [in] size given mmap size in bytes for virtual CPU state.
        ///
        /// @return None - in case of success.
        /// @return VmmError - otherwise.
        auto init(i32 raw, usize size) noexcept -> VmmResult<None>;
    };

}

#endif // NULLVM_CORE_VCPU_HPP
