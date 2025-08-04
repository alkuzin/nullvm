// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// Virtual machine related declarations.

#include <nullvm/core/vm.hpp>

namespace nullvm::core {

    auto VirtualMachine::init() noexcept -> VmmResult<None> {

        if (auto result = this->kvm.init(); !result) {
            return result;
        }

        auto result = this->kvm.create_vm();

        if (!result) {
            return std::unexpected(result.error());
        }

        this->vmfd.init(result.value());

        return None {};
    }

}