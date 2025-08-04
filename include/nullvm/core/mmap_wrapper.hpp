// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// Value wrapper for mmap syscall related declarations.

#ifndef NULLVM_CORE_MMAP_WRAPPER_HPP
#define NULLVM_CORE_MMAP_WRAPPER_HPP

#include <nullvm/types.hpp>

namespace nullvm::core {

    /// Value wrapper for mmap syscall.
    struct MMapWrapper {
        /// Mapped data memory address.
        void *addr;
        /// Mapped data size in bytes.
        usize size;

        /// @brief Construct new MMapWrapper object.
        MMapWrapper() noexcept: addr(nullptr), size(0) {}

        /// @brief Destruct MMapWrapper object.
        ~MMapWrapper() noexcept;

        /// @brief Initialize MMapWrapper object.
        ///
        /// @param [in] addr given mapped data memory address.
        /// @param [in] size given mapped data size in bytes.
        ///
        /// @return None - in case of success.
        /// @return VmmError - otherwise.
        auto init(void *addr, usize size) noexcept -> VmmResult<None>;
    };

}

#endif // NULLVM_CORE_MMAP_WRAPPER_HPP
