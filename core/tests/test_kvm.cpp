// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// Kernel-based Virtual Machine (KVM) subsystem handle tests.

#include <nullvm/core/utils/utils.hpp>
#include <nullvm/core/kvm.hpp>
#include <gtest/gtest.h>
#include <fcntl.h>

using namespace nullvm::core;

TEST(test_kvm, test_kvm_creation) {
    Kvm kvm;
    const auto result = kvm.init();
    EXPECT_TRUE(result.has_value());
}

TEST(test_kvm, test_kvm_vm_creation) {
    Kvm kvm;
    const auto result = kvm.init();
    EXPECT_TRUE(result.has_value());

    const auto vmfd_result = kvm.create_vm();

    GTEST_LOG_(INFO) << vmfd_result.error();
    EXPECT_TRUE(vmfd_result.has_value());
}

TEST(test_kvm, test_kvm_destruction) {
    Kvm kvm;
    const auto result = kvm.init();
    EXPECT_TRUE(result.has_value());

    const auto fd = kvm.fd();
    kvm.~Kvm();

    EXPECT_FALSE(utils::is_fd_open(fd));
}
