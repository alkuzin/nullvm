// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// KVM virtual CPU file descriptor handle tests.

#include <nullvm/core/vcpu.hpp>
#include <gtest/gtest.h>
#include <fcntl.h>

using namespace nullvm::core;

/// @brief Check whether file descriptor is open.
///
/// @param [in] fd given file descriptor to check.
///
/// @return true - if file descriptor is open.
/// @return false - otherwise.
static auto is_fd_open(const nullvm::i32 fd) -> bool {
    return fcntl(fd, F_GETFD) != -1;
}

TEST(test_vcpu, test_vcpu_creation_correct) {
    VCpu vcpu;

    const auto raw_fd = open("/dev/null", O_RDWR);
    const auto result = vcpu.init(raw_fd);

    EXPECT_TRUE(result.has_value());
}

TEST(test_vcpu, test_vcpu_closing_file_descriptor) {
    VCpu vcpu;

    const auto raw_fd = open("/dev/null", O_RDWR);
    const auto result = vcpu.init(raw_fd);

    EXPECT_TRUE(result.has_value());

    const auto fd = vcpu.raw;
    vcpu.~VCpu();

    EXPECT_FALSE(is_fd_open(fd));
    EXPECT_EQ(vcpu.raw, -1);
}

TEST(test_vcpu, test_vcpu_creation_negative_fd) {
    VCpu vcpu;
    const auto result = vcpu.init(-1);

    EXPECT_FALSE(result.has_value());
    GTEST_LOG_(INFO) << result.error();
}
