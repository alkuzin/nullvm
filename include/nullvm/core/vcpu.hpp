// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// KVM virtual CPU file descriptor handle.

#ifndef NULLVM_CORE_VCPU_HPP
#define NULLVM_CORE_VCPU_HPP

#include <nullvm/core/utils/mmap_wrapper.hpp>
#include <nullvm/core/utils/fd_wrapper.hpp>
#include <nullvm/types.hpp>
#include <linux/kvm.h>

namespace nullvm::core {
    using utils::FDWrapper;
    using utils::MMapWrapper;

    /// Virtual CPU file descriptor management struct.
    class VCpu final {
        /// Virtual CPU file descriptor.
        FDWrapper m_fd;
        /// Virtual CPU state.
        MMapWrapper m_state;
        /// Virtual CPU's special registers.
        kvm_sregs m_sregs;
        /// Virtual CPU's standard registers.
        kvm_regs m_regs;

    public:
        /// @brief Initialize VCpu object.
        ///
        /// @param [in] fd given raw virtual CPU file descriptor.
        /// @param [in] size given mmap size in bytes for virtual CPU state.
        ///
        /// @return None - in case of success.
        /// @return VmmError - otherwise.
        auto init(i32 fd, usize size) noexcept -> VmmResult<None>;

        /// @brief Get special register state of virtual CPU.
        ///
        /// @return Special registers - in case of success.
        /// @return VmmError - otherwise.
        auto sregs() noexcept -> VmmResult<kvm_sregs>;

        /// @brief Set special register state of virtual CPU.
        ///
        /// @param [in] sregs given special registers to set.
        ///
        /// @return None - in case of success.
        /// @return VmmError - otherwise.
        auto set_sregs(const kvm_sregs& sregs) noexcept -> VmmResult<None>;

        /// @brief Get standard register state of virtual CPU.
        ///
        /// @return Special registers - in case of success.
        /// @return VmmError - otherwise.
        auto regs() noexcept -> VmmResult<kvm_regs>;

        /// @brief Set standard register state of virtual CPU.
        ///
        /// @param [in] regs given standard registers to set.
        ///
        /// @return Standard registers - in case of success.
        /// @return VmmError - otherwise.
        auto set_regs(const kvm_regs& regs) noexcept -> VmmResult<None>;

        /// @brief Get virtual CPU state info.
        ///
        /// @return Virtual CPU state info.
        auto state() noexcept -> kvm_run*;

        /// @brief Run virtual CPU.
        ///
        /// @return None - in case of success.
        /// @return VmmError - otherwise.
        auto run() noexcept -> VmmResult<None>;

    private:
        /// @brief Set initial register state of virtual CPU.
        ///
        /// @return None - in case of success.
        /// @return VmmError - otherwise.
        auto setup_registers() noexcept -> VmmResult<None>;
    };

}

#endif // NULLVM_CORE_VCPU_HPP
