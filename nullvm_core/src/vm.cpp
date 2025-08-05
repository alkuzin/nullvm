// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// Virtual machine related declarations.

#include <nullvm/core/vm.hpp>
#include <linux/kvm.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <cstring>

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

        auto vcpu_result = this->vmfd.create_vcpu();

        if (!vcpu_result) {
            return std::unexpected(vcpu_result.error());
        }

        const auto vcpufd = vcpu_result.value();
        const auto size = static_cast<usize>(
            ioctl(this->kvm.raw, KVM_GET_VCPU_MMAP_SIZE, 0)
        );

        if (auto result = this->vcpu.init(vcpufd, size); !result) {
            return std::unexpected(result.error());
        }

        return None {};
    }

    auto VirtualMachine::set_vm_memory(usize size) noexcept -> VmmResult<None> {

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

    auto VirtualMachine::set_mem_region(u64 addr, usize size) noexcept
    -> VmmResult<None> {

        if (const auto result = this->set_vm_memory(size); !result) {
            return std::unexpected(result.error());
        }

        kvm_userspace_memory_region mem_region = {
            // Memory slot number.
            .slot = 0,
            // Flags that specify attributes of the memory region.
            .flags = 0,
            // Starting physical address in the guest's address space where
            // this memory region is mapped.
            .guest_phys_addr = addr,
            // Size of the memory region in bytes.
            .memory_size = size,
            // Starting address of the memory allocated in userspace that
            // will be mapped to the guest's physical address.
            .userspace_addr = reinterpret_cast<u64>(this->memory.addr),
        };

        this->vcpu.regs.rip = addr;

        const auto ret = ioctl(
            this->vmfd.raw,
            KVM_SET_USER_MEMORY_REGION,
            &mem_region
        );

        if (ret == -1) {
            return std::unexpected("Cannot set user memory region");
        }

        return None {};
    }

    auto VirtualMachine::load_raw(const std::vector<u8>& raw) noexcept
    -> VmmResult<None> {

        const auto size = raw.size();

        if (size == 0)
            return std::unexpected("Raw binary size is zero");

        std::memcpy(this->memory.addr, raw.data(), size);
        return None {};
    }

}