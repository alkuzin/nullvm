// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// Value wrapper for mmap syscall tests.

#include <nullvm/core/mmap_wrapper.hpp>
#include <gtest/gtest.h>
#include <sys/mman.h>

using namespace nullvm::core;

TEST(test_mmap_wrapper, test_mmap_wrapper_creation_correct) {
    MMapWrapper wrapper;

    const auto prot  = PROT_READ | PROT_WRITE;
    const auto flags = MAP_SHARED | MAP_ANONYMOUS;
    const auto size  = 0x1000;

    auto addr = mmap(nullptr, size, prot, flags, -1, 0);
    const auto result = wrapper.init(addr, size);

    EXPECT_TRUE(result.has_value());
}

TEST(test_mmap_wrapper, test_mmap_wrapper_creation_incorrect_data) {
    MMapWrapper wrapper;

    const auto prot  = PROT_READ | PROT_WRITE;
    const auto flags = MAP_SHARED | MAP_ANONYMOUS;
    const auto size  = 0x1000;

    [[maybe_unused]]
    auto addr = mmap(nullptr, size, prot, flags, -1, 0);
    const auto result = wrapper.init(nullptr, size);

    EXPECT_FALSE(result.has_value());
    GTEST_LOG_(INFO) << "Result: " << result.error();
}

TEST(test_mmap_wrapper, test_mmap_wrapper_creation_incorrect_size) {
    MMapWrapper wrapper;

    const auto prot  = PROT_READ | PROT_WRITE;
    const auto flags = MAP_SHARED | MAP_ANONYMOUS;
    const auto size  = 0x1000;

    auto addr = mmap(nullptr, size, prot, flags, -1, 0);
    const auto result = wrapper.init(addr, 0);

    EXPECT_FALSE(result.has_value());
    GTEST_LOG_(INFO) << "Result: " << result.error();
}

TEST(test_mmap_wrapper, test_mmap_wrapper_destructing_correct) {
    MMapWrapper wrapper;

    const auto prot  = PROT_READ | PROT_WRITE;
    const auto flags = MAP_SHARED | MAP_ANONYMOUS;
    const auto size  = 0x1000;

    auto addr = mmap(nullptr, size, prot, flags, -1, 0);
    const auto result = wrapper.init(addr, size);
    wrapper.~MMapWrapper();

    EXPECT_EQ(wrapper.addr, nullptr);
    EXPECT_EQ(wrapper.size, 0);
}
