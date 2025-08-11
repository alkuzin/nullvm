// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// Value wrapper for raw file descriptor tests.

#include <nullvm/core/fd_wrapper.hpp>
#include <gtest/gtest.h>
#include <fcntl.h>

using namespace nullvm::core;

// TODO: move to utils from this and other tests.
namespace {
    /// @brief Check whether file descriptor is open.
    ///
    /// @param [in] fd given file descriptor to check.
    ///
    /// @return true - if file descriptor is open.
    /// @return false - otherwise.
    auto is_fd_open(const nullvm::i32 fd) -> bool {
        return fcntl(fd, F_GETFD) != -1;
    }
}

TEST(test_fd_wrapper, test_fd_wrapper_fd_method) {
    auto fd = open("/dev/null", O_RDONLY);
    auto wrapper = FDWrapper(fd);
    auto read_fd = wrapper.fd();

    EXPECT_EQ(fd, read_fd);
}

TEST(test_fd_wrapper, test_fd_wrapper_close_fd) {
    auto fd = open("/dev/null", O_RDONLY);
    auto wrapper = FDWrapper(fd);
    wrapper.~FDWrapper();

    EXPECT_FALSE(is_fd_open(fd));
}
