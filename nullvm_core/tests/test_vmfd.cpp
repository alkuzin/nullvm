// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// KVM virtual machine file descriptor handle tests.

#include <nullvm/core/vmfd.hpp>
#include <gtest/gtest.h>
#include <fcntl.h>

using namespace nullvm::core;

/// @brief Check whether file descriptor is open.
///
/// @param [in] fd given file descriptor to check.
///
/// @return true - if file descriptor is open.
/// @return false - otherwise.
auto is_fd_open(const nullvm::i32 fd) -> bool {
    return fcntl(fd, F_GETFD) != -1;
}

TEST(test_vmfd, test_vmfd_creation_correct) {
    const auto raw_fd = open("/dev/null", O_RDWR);
    const auto result = VmFd::create(raw_fd);

    EXPECT_TRUE(result.has_value());
}

TEST(test_vmfd, test_vmfd_closing_file_descriptor) {
    const auto raw_fd = open("/dev/null", O_RDWR);
    const auto result = VmFd::create(raw_fd);

    EXPECT_TRUE(result.has_value());

    const auto& vmfd = result.value();
    const auto fd = vmfd.raw;
    vmfd.~VmFd();

    EXPECT_FALSE(is_fd_open(fd));
    EXPECT_EQ(vmfd.raw, -1);
}

TEST(test_vmfd, test_vmfd_creation_negative_fd) {
    const auto result = VmFd::create(-1);

    EXPECT_FALSE(result.has_value());
    GTEST_LOG_(INFO) << result.error();
}

TEST(test_vmfd, test_vmfd_creation_standard_fd) {
    const auto result0 = VmFd::create(0);
    const auto result1 = VmFd::create(1);
    const auto result2 = VmFd::create(2);

    EXPECT_FALSE(result0.has_value());
    GTEST_LOG_(INFO) << result0.error();

    EXPECT_FALSE(result1.has_value());
    GTEST_LOG_(INFO) << result1.error();

    EXPECT_FALSE(result2.has_value());
    GTEST_LOG_(INFO) << result2.error();
}
