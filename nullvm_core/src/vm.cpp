// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// Virtual machine related declarations.

#include <nullvm/core/vm.hpp>
#include <nullvm/log.hpp>
#include <linux/kvm.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <bit>

namespace nullvm::core {

    auto VirtualMachine::init() noexcept -> VmmResult<None> {
        if (auto result = m_kvm.init(); !result)
            return result;

        auto vmfd_result = m_kvm.create_vm();

        if (!vmfd_result)
            return std::unexpected(vmfd_result.error());

        if (auto result = m_vmfd.init(vmfd_result.value()); !result)
            return std::unexpected(result.error());

        auto vcpu_result = m_vmfd.create_vcpu();

        if (!vcpu_result)
            return std::unexpected(vcpu_result.error());

        const auto vcpufd = vcpu_result.value();

        // TODO: implement Kvm method for that.
        const auto size = static_cast<usize>(
            ioctl(m_kvm.fd(), KVM_GET_VCPU_MMAP_SIZE, 0)
        );

        if (auto result = m_vcpu.init(vcpufd, size); !result)
            return std::unexpected(result.error());

        return None {};
    }

    auto VirtualMachine::vcpu() & noexcept -> VCpu& {
        return m_vcpu;
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

        if (auto result = m_memory.init(addr, size); !result)
            return result;

        return None {};
    }

    auto VirtualMachine::set_mem_region(u64 addr, usize size) noexcept
    -> VmmResult<None> {
        if (auto result = set_vm_memory(size); !result)
            return std::unexpected(result.error());

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
            .userspace_addr = std::bit_cast<u64>(m_memory.addr()),
        };

        // Set virtual CPU registers.
        auto result = m_vcpu.regs();

        if (!result)
            return std::unexpected(result.error());

        auto regs = result.value();
        regs.rip = addr;

        if (auto result = m_vcpu.set_regs(regs); !result)
            return std::unexpected(result.error());

        // TODO: move to VmFd.
        const auto ret = ioctl(
            m_vmfd.fd(),
            KVM_SET_USER_MEMORY_REGION,
            &mem_region
        );

        if (ret == -1)
            return std::unexpected("Cannot set user memory region");

        return None {};
    }

    auto VirtualMachine::load_raw(const std::vector<u8>& raw) noexcept
    -> VmmResult<None> {
        const auto size = raw.size();

        if (size == 0)
            return std::unexpected("Raw binary size is zero");

        auto addr = static_cast<u8*>(m_memory.addr());
        auto data = raw.data();
        std::copy(data, data + size, addr);

        return None {};
    }

    auto VirtualMachine::run() noexcept -> VmmResult<None> {
        auto state = m_vcpu.state();

        while (true) {
            if (auto result = m_vcpu.run(); !result)
                return std::unexpected(result.error());

            log::debug("Exit reason: {}", state->exit_reason);
            switch (state->exit_reason) {

                case KVM_EXIT_HLT:
                    log::debug("KVM_EXIT_HLT");
                    return None {};

                case KVM_EXIT_IO:
                    log::debug("KVM_EXIT_IO");

                    if (auto result = handle_exit_io(state); !result)
                        return std::unexpected(result.error());

                    break;

                default:
                    log::debug("Unhandled exit reason: {}", state->exit_reason);
                    return std::unexpected("Unhandled exit reason");
            }
        }

    }

    auto VirtualMachine::handle_exit_io(const kvm_run *state) noexcept
    -> VmmResult<None> {
        auto io = state->io;

        if (io.direction == KVM_EXIT_IO_IN) {
            log::debug("PORT IN ({:#x})", io.port);
        }
        else if (state->io.direction == KVM_EXIT_IO_OUT) {
            log::debug("PORT OUT ({:#x})", io.port);

            if (io.port == 0x3f8 && io.size == 1 && io.count == 1) {
                const auto byte = *(
                    std::bit_cast<const u8*>(state) + io.data_offset
                );

                std::putchar(byte);
            }
        }

        return None {};
    }
}