// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// KVM virtual CPU file descriptor handle.

#include <nullvm/core/vcpu.hpp>
#include <linux/kvm.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

namespace nullvm::core {

    auto VCpu::init(i32 raw, usize size) noexcept -> VmmResult<None> {

        if (raw < 0) {
            const auto err = "Invalid file descriptor: must be non-negative";
            return std::unexpected(err);
        }

        if (raw == 0 || raw == 1 || raw == 2) {
            const auto err = "Invalid file descriptor: cannot be 0, 1, or 2 "
            "(stdin, stdout, stderr)";
            return std::unexpected(err);
        }

        if (size == 0) {
            return std::unexpected(
                "Error to init virtual CPU: mapping memory size is zero"
            );
        }

        this->raw = raw;

        const auto prot = PROT_READ | PROT_WRITE;
        const auto addr = mmap(nullptr, size, prot, MAP_SHARED, raw, 0);

        if(auto result = this->state.init(addr, size); !result) {
            return std::unexpected(result.error());
        }

        if (auto result = this->set_registers(); !result) {
            return std::unexpected(result.error());
        }

        return None {};
    }

    VCpu::~VCpu() noexcept {
        if (this->raw != -1) {
            close(this->raw);
            this->raw = -1;
        }
    }

    auto VCpu::set_registers() noexcept -> VmmResult<None> {

        if (auto result = this->get_sregs(); !result) {
            return std::unexpected(result.error());
        }

        this->sregs.cs.base     = 0;
        this->sregs.cs.selector = 0;

        if (auto result = this->set_sregs(); !result) {
            return std::unexpected(result.error());
        }

        this->regs.rflags = 0x2;

        if (auto result = this->set_regs(); !result) {
            return std::unexpected(result.error());
        }

        return None {};
    }

    auto VCpu::get_sregs() noexcept -> VmmResult<None> {

        const auto ret = ioctl(this->raw, KVM_GET_SREGS, &this->sregs);

        if (ret == -1) {
            return std::unexpected("Error to get special registers state");
        }

        return None {};
    }

    auto VCpu::set_sregs() noexcept -> VmmResult<None> {

        const auto ret = ioctl(this->raw, KVM_SET_SREGS, &this->sregs);

        if (ret == -1) {
            return std::unexpected("Error to set special registers state");
        }

        return None {};
    }

    auto VCpu::set_regs() noexcept -> VmmResult<None> {

        const auto ret = ioctl(this->raw, KVM_SET_REGS, &this->regs);

        if (ret == -1) {
            return std::unexpected("Error to set standard registers state");
        }

        return None {};
    }
}
