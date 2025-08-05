// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// Virtual machine related declarations tests.

#include "nullvm/types.hpp"
#include "gtest/gtest.h"
#include <nullvm/core/vm.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <array>
#include <print>

using namespace nullvm::core;
using namespace nullvm;

TEST(test_vm, test_vm_creation_correct) {
    VirtualMachine vm;
    const auto result = vm.init();

    EXPECT_TRUE(result.has_value());
}

TEST(test_vm, test_vm_set_mem_region_correct) {
    VirtualMachine vm;

    auto result = vm.init();
    EXPECT_TRUE(result.has_value());

    result = vm.set_mem_region(0x1000, 0x1000);
    EXPECT_TRUE(result.has_value());
}

TEST(test_vm, test_vm_set_mem_region_incorrect_size) {
    VirtualMachine vm;

    auto result = vm.init();
    EXPECT_TRUE(result.has_value());

    result = vm.set_mem_region(0x1000, 0);
    EXPECT_FALSE(result.has_value());
}

TEST(test_vm, test_vm_load_raw_correct) {
    VirtualMachine vm;

    auto result = vm.init();
    EXPECT_TRUE(result.has_value());

    result = vm.set_mem_region(0x1000, 0x1000);
    EXPECT_TRUE(result.has_value());

    const std::array<u8, 12> code = {
        0xba, 0xf8, 0x03,   // mov $0x3f8, %dx
        0x00, 0xd8,         // add %bl, %al
        0x04, '0',          // add $'0', %al
        0xee,               // out %al, (%dx)
        0xb0, '\n',         // mov $'\n', %al
        0xee,               // out %al, (%dx)
        0xf4,               // hlt
    };

    const auto code_vec = std::vector<u8>(code.begin(), code.end());

    result = vm.load_raw(code_vec);
    EXPECT_TRUE(result.has_value());

    auto data = reinterpret_cast<u8*>(vm.memory.addr);

    for (usize i = 0; i < code.size(); i++) {
        EXPECT_EQ(code[i], data[i]);
    }
}
