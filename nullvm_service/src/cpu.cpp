// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// x86 CPU related declarations.

#include <nullvm/service/cpu.hpp>
#include <cstring>

namespace nullvm::service {

    /// Size of CPU vendor ID in bytes.
    constexpr u8 CPUID_VENDOR_ID_SIZE {13};

    /// Size of CPU brand in bytes.
    constexpr u8 CPUID_BRAND_SIZE {48};

    auto get_cpu_vendor() -> std::string {
        std::string vendor_id;
        vendor_id.reserve(CPUID_VENDOR_ID_SIZE);

        auto cpu_info = cpuid(0);
        vendor_id.append(reinterpret_cast<char*>(&cpu_info.ebx), 4);
        vendor_id.append(reinterpret_cast<char*>(&cpu_info.ecx), 4);
        vendor_id.append(reinterpret_cast<char*>(&cpu_info.edx), 4);

        return vendor_id;
    }

    auto get_cpu_brand() -> std::string {
        std::string brand(CPUID_BRAND_SIZE, '\0');
        CpuidInfo cpu_info {};

        cpu_info = cpuid(0x80000002);
        std::memcpy(&brand[0], &cpu_info, sizeof(cpu_info));

        cpu_info = cpuid(0x80000003);
        std::memcpy(&brand[16], &cpu_info, sizeof(cpu_info));

        cpu_info = cpuid(0x80000004);
        std::memcpy(&brand[32], &cpu_info, sizeof(cpu_info));

        // Remove trailing null characters.
        auto last_non_null = brand.find_last_not_of('\0');

        if (last_non_null != std::string::npos)
            brand.resize(last_non_null + 1);
        else
            brand = "Unknown CPU brand";

        return brand;
    }

    auto is_virtualization_supported() -> bool {
        const auto cpu_info = cpuid(1);
        return (cpu_info.ecx & (1 << 5)) != 0;
    }

}
