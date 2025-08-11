// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// KVM virtual machine file descriptor handle.

#ifndef NULLVM_CORE_VMFD_HPP
#define NULLVM_CORE_VMFD_HPP

#include <nullvm/core/fd_wrapper.hpp>
#include <nullvm/types.hpp>

namespace nullvm::core {

    /// Virtual machine file descriptor management struct.
    class VmFd final {
        /// Virtual machine file descriptor.
        FDWrapper m_fd;

    public:
        /// @brief Initialize VmFd object.
        ///
        /// @param [in] fd given raw virtual machine file descriptor.
        ///
        /// @return None - in case of success.
        /// @return VmmError - otherwise.
        auto init(i32 fd) noexcept -> VmmResult<None>;

        /// @brief Get raw file descriptor value.
        ///
        /// @return Raw file descriptor value.
        auto fd() const noexcept -> i32;

        /// @brief Create virtual CPU.
        ///
        /// @return New virtual CPU file descriptor - in case of success.
        /// @return VmmError - otherwise.
        auto create_vcpu() const -> VmmResult<i32>;
    };

}

#endif // NULLVM_CORE_VMFD_HPP
