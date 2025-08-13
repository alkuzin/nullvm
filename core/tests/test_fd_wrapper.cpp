// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// Value wrapper for raw file descriptor tests.

#include <nullvm/core/utils/fd_wrapper.hpp>
#include <nullvm/core/utils/utils.hpp>
#include <gtest/gtest.h>
#include <fcntl.h>

using namespace nullvm::core;
using utils::FDWrapper;

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

    EXPECT_FALSE(utils::is_fd_open(fd));
}
