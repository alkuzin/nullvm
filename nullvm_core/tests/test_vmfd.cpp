// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// KVM virtual machine file descriptor handle tests.

#include <nullvm/core/vmfd.hpp>
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

TEST(test_vmfd, test_vmfd_creation_correct) {
    VmFd vmfd;

    const auto raw_fd = open("/dev/null", O_RDWR);
    const auto result = vmfd.init(raw_fd);

    EXPECT_TRUE(result.has_value());
}

TEST(test_vmfd, test_vmfd_closing_file_descriptor) {
    VmFd vmfd;

    const auto raw_fd = open("/dev/null", O_RDWR);
    const auto result = vmfd.init(raw_fd);

    EXPECT_TRUE(result.has_value());

    const auto fd = vmfd.fd();
    vmfd.~VmFd();

    EXPECT_FALSE(is_fd_open(fd));
}

TEST(test_vmfd, test_vmfd_creation_negative_fd) {
    VmFd vmfd;
    const auto result = vmfd.init(-1);

    EXPECT_FALSE(result.has_value());
    GTEST_LOG_(INFO) << result.error();
}

TEST(test_vmfd, test_vmfd_creation_standard_fd) {
    VmFd vmfd0, vmfd1, vmfd2;

    const auto result0 = vmfd0.init(0);
    const auto result1 = vmfd1.init(1);
    const auto result2 = vmfd2.init(2);

    EXPECT_FALSE(result0.has_value());
    GTEST_LOG_(INFO) << result0.error();

    EXPECT_FALSE(result1.has_value());
    GTEST_LOG_(INFO) << result1.error();

    EXPECT_FALSE(result2.has_value());
    GTEST_LOG_(INFO) << result2.error();
}
