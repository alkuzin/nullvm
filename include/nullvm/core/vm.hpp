// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// Virtual machine related declarations.

#ifndef NULLVM_CORE_VM_HPP
#define NULLVM_CORE_VM_HPP

#include <nullvm/core/mmap_wrapper.hpp>
#include <nullvm/core/vcpu.hpp>
#include <nullvm/core/kvm.hpp>
#include <vector>

namespace nullvm::core {

    /// Virtual machine info struct.
    struct VirtualMachine {
        /// KVM subsystem handle.
        Kvm kvm;
        /// Virtual machine file descriptor.
        VmFd vmfd;
        /// Memory allocated to VM.
        MMapWrapper memory;
        /// Virtual CPU handle.
        VCpu vcpu;

        /// @brief Construct new VirtualMachine object.
        VirtualMachine() noexcept = default;

        /// @brief Initialize VirtualMachine object.
        ///
        /// @return None - in case of success.
        /// @return VmmError - otherwise.
        auto init() noexcept -> VmmResult<None>;

        /// @brief Set userspace memory region.
        ///
        /// @param addr given guest's starting address.
        /// @param size given size of the memory region in bytes.
        ///
        /// @return None - in case of success.
        /// @return VmmError - otherwise.
        auto set_mem_region(u64 addr, usize size) noexcept -> VmmResult<None>;

        /// @brief Load raw binary contents to VM's memory.
        ///
        /// @param raw given raw binary bytes to load.
        ///
        /// @return None - in case of success.
        /// @return VmmError - otherwise.
        auto load_raw(const std::vector<u8>& raw) noexcept -> VmmResult<None>;

        /// @brief Run virtual machine.
        ///
        /// @return None - in case of success.
        /// @return VmmError - otherwise.
        auto run() noexcept -> VmmResult<None>;

    private:
        /// @brief Set VM's memory.
        ///
        /// @param size given size of the memory region in bytes to allocate.
        ///
        /// @return None - in case of success.
        /// @return VmmError - otherwise.
        auto set_vm_memory(usize size) noexcept -> VmmResult<None>;

        /// @brief Handle VM exit on I/O port.
        ///
        /// @param state given virtual CPU state.
        ///
        /// @return None - in case of success.
        /// @return VmmError - otherwise.
        auto handle_exit_io(const kvm_run *state) noexcept -> VmmResult<None>;
    };

}

#endif // NULLVM_CORE_VM_HPP
