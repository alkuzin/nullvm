// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// Value wrapper for raw file descriptor related declarations.

#include <nullvm/core/fd_wrapper.hpp>

namespace nullvm::core {

    auto FDDeleter::operator()(i32 *fd) const noexcept -> void {
        if (fd && *fd != -1) {
            close(*fd);
            delete fd;
        }
    }

    auto FDWrapper::fd() const noexcept -> i32 {
        return *m_fd.get();
    }
}
