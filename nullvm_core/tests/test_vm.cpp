// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// Virtual machine related declarations tests.

#include <nullvm/core/vm.hpp>
#include <gtest/gtest.h>

using namespace nullvm::core;

TEST(test_vm, test_vm_creation_correct) {
    VirtualMachine vm;
    const auto result = vm.init();

    EXPECT_TRUE(result.has_value());
}

TEST(test_vm, test_vm_set_vm_memory_correct) {
    VirtualMachine vm;

    auto result = vm.init();
    EXPECT_TRUE(result.has_value());

    result = vm.set_vm_memory(0x1000);
    EXPECT_TRUE(result.has_value());
}

TEST(test_vm, test_vm_set_vm_memory_incorrect_size) {
    VirtualMachine vm;

    auto result = vm.init();
    EXPECT_TRUE(result.has_value());

    result = vm.set_vm_memory(0);
    EXPECT_FALSE(result.has_value());
}
