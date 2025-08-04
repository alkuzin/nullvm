// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// Virtual machine related declarations.

#include <nullvm/core/vm.hpp>
#include <sys/mman.h>

namespace nullvm::core {

    auto VirtualMachine::init() noexcept -> VmmResult<None> {
        if (auto result = this->kvm.init(); !result) {
            return result;
        }

        auto vmfd_result = this->kvm.create_vm();

        if (!vmfd_result) {
            return std::unexpected(vmfd_result.error());
        }

        if (auto result = this->vmfd.init(vmfd_result.value()); !result) {
            return std::unexpected(result.error());
        }

        return None {};
    }

    auto VirtualMachine::set_vm_memory(const usize size) noexcept
    -> VmmResult<None> {
        if (size == 0) {
            return std::unexpected(
                "Error to set VM's memory: mapping memory size is zero"
            );
        }

        // Mapping protection flags.
        auto prot = PROT_READ | PROT_WRITE;

        // MAP_SHARED flag ensures that both the host and guest can see the
        // same changes to the memory.
        // MAP_ANONYMOUS flag indicates that the mapping is not
        // backed by any file.
        auto flags = MAP_SHARED | MAP_ANONYMOUS;

        auto addr = mmap(nullptr, size, prot, flags, -1, 0);

        if (auto result = this->memory.init(addr, size); !result) {
            return result;
        }

        return None {};
    }

}