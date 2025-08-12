// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// Value wrapper for raw file descriptor related declarations.

#ifndef NULLVM_CORE_UTILS_FD_WRAPPER_HPP
#define NULLVM_CORE_UTILS_FD_WRAPPER_HPP

#include <nullvm/types.hpp>
#include <memory>

namespace nullvm::core::utils {

    /// Raw file descriptor deleter struct.
    struct FDDeleter final {
        /// @brief Close file descriptor.
        ///
        /// @param [in] fd given raw file descriptor.
        auto operator()(i32 *fd) const noexcept -> void;
    };

    /// Value wrapper for raw file descriptor.
    class FDWrapper final {
        /// File descriptor.
        std::unique_ptr<i32, FDDeleter> m_fd;

    public:
        /// @brief Construct new FDWrapper object.
        FDWrapper() noexcept : m_fd(new i32(-1), FDDeleter()) {}

        /// @brief Construct new FDWrapper object.
        ///
        /// @param fd given raw file descriptor.
        explicit FDWrapper(i32 fd) noexcept : m_fd(new i32(fd), FDDeleter()) {}

        /// @brief Get raw file descriptor value.
        ///
        /// @return Raw file descriptor value.
        auto fd() const noexcept -> i32;
    };

}

#endif // NULLVM_CORE_UTILS_FD_WRAPPER_HPP