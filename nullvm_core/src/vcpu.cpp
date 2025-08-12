// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// KVM virtual CPU file descriptor handle.

#include <nullvm/core/vcpu.hpp>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

namespace nullvm::core {

    auto VCpu::init(i32 fd, usize size) noexcept -> VmmResult<None> {
        if (fd < 0) {
            return std::unexpected(
                "Invalid file descriptor: must be non-negative"
            );
        }

        if (fd == 0 || fd == 1 || fd == 2) {
            return std::unexpected(
                "Invalid file descriptor: cannot be 0, 1, or 2 "
                "(stdin, stdout, stderr)"
            );
        }

        if (size == 0) {
            return std::unexpected(
                "Error to init virtual CPU: mapping memory size is zero"
            );
        }

        m_fd = FDWrapper(fd);

        const auto prot = PROT_READ | PROT_WRITE;
        const auto addr = mmap(nullptr, size, prot, MAP_SHARED, fd, 0);

        if(auto result = m_state.init(addr, size); !result)
            return std::unexpected(result.error());

        if (auto result = setup_registers(); !result)
            return std::unexpected(result.error());

        return None {};
    }

    auto VCpu::setup_registers() noexcept -> VmmResult<None> {
        auto result = sregs();

        if (!result)
            return std::unexpected(result.error());

        m_sregs = result.value();
        m_sregs.cs.base = 0;
        m_sregs.cs.selector = 0;

        if (auto result = set_sregs(m_sregs); !result)
            return std::unexpected(result.error());

        m_regs.rflags = 0x2;

        if (auto result = set_regs(m_regs); !result)
            return std::unexpected(result.error());

        return None {};
    }

    auto VCpu::sregs() noexcept -> VmmResult<kvm_sregs> {
        const auto ret = ioctl(m_fd.fd(), KVM_GET_SREGS, &m_sregs);

        if (ret == -1)
            return std::unexpected("Error to get special registers state");

        return m_sregs;
    }

    auto VCpu::set_sregs(const kvm_sregs& sregs) noexcept -> VmmResult<None> {
        const auto ret = ioctl(m_fd.fd(), KVM_SET_SREGS, &sregs);

        if (ret == -1)
            return std::unexpected("Error to set special registers state");

        return None {};
    }

    auto VCpu::regs() noexcept -> VmmResult<kvm_regs> {
        const auto ret = ioctl(m_fd.fd(), KVM_GET_REGS, &m_regs);

        if (ret == -1)
            return std::unexpected("Error to get standard registers state");

        return m_regs;
    }

    auto VCpu::set_regs(const kvm_regs& regs) noexcept -> VmmResult<None> {
        const auto ret = ioctl(m_fd.fd(), KVM_SET_REGS, &regs);

        if (ret == -1)
            return std::unexpected("Error to set standard registers state");

        return None {};
    }

    auto VCpu::state() noexcept -> kvm_run* {
        return std::bit_cast<kvm_run*>(m_state.addr());
    }

    auto VCpu::run() noexcept -> VmmResult<None> {
        const auto ret = ioctl(m_fd.fd(), KVM_RUN, 0);

        if (ret == -1)
            return std::unexpected("Error to run virtual machine");

        return None {};
    }

}
