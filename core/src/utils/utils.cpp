// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// Auxilar functions declarations.

#include <nullvm/core/utils/utils.hpp>
#include <fcntl.h>

namespace nullvm::core::utils {

    auto is_fd_open(const nullvm::i32 fd) -> bool {
        return fcntl(fd, F_GETFD) != -1;
    }

}
