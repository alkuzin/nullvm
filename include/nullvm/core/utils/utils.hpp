// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// Auxilar functions declarations.

#ifndef NULLVM_CORE_UTILS_HPP
#define NULLVM_CORE_UTILS_HPP

#include <nullvm/types.hpp>

namespace nullvm::core::utils {

    /// @brief Check whether file descriptor is open.
    ///
    /// @param [in] fd given file descriptor to check.
    ///
    /// @return true - if file descriptor is open.
    /// @return false - otherwise.
    auto is_fd_open(const nullvm::i32 fd) -> bool;

}

#endif // NULLVM_CORE_UTILS_HPP
