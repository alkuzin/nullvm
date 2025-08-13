// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// x86 CPU related declarations.

#ifndef NULLVM_CORE_CPU_HPP
#define NULLVM_CORE_CPU_HPP

#include <nullvm/types.hpp>

namespace nullvm::core {

    /// CPUID information registers struct.
    struct CpuidInfo {
        /// EAX register to store CPU information.
        u32 eax;
        /// EBX register to store CPU information.
        u32 ebx;
        /// ECX register to store CPU information.
        u32 ecx;
        /// EDX register to store CPU information.
        u32 edx;
    };

    /// @brief Set CPU info registers.
    ///
    /// @param [in] leaf given category of CPU information to gather.
    /// @return CPUID information registers struct.
    inline auto cpuid(u32 leaf) -> CpuidInfo {
        CpuidInfo info;

        asm (
            "cpuid"
            : "=a"(info.eax), "=b"(info.ebx), "=c"(info.ecx), "=d"(info.edx)
            : "a"(leaf)
        );

        return info;
    }

    /// @brief Get CPU vendor info.
    ///
    /// @return CPU vendor ID in string representation.
    auto get_cpu_vendor() -> std::string;

    /// @brief Get CPU brand.
    ///
    /// @return CPU brand in string representation.
    auto get_cpu_brand() -> std::string;

    /// @brief Check whether CPU supporting virtualization.
    ///
    /// @return true if CPU supporting virtualization.
    /// @return false otherwise.
    auto is_virtualization_supported() -> bool;

}

#endif // NULLVM_CORE_CPU_HPP
