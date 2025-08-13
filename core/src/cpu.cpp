// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// x86 CPU related declarations.

#include <nullvm/core/cpu.hpp>
#include <bit>

namespace nullvm::service {

    namespace {
        /// Size of CPU vendor ID in bytes.
        constexpr u8 CPUID_VENDOR_ID_SIZE {13};

        /// Size of CPU brand in bytes.
        constexpr u8 CPUID_BRAND_SIZE {48};
    }

    auto get_cpu_vendor() -> std::string {
        std::string vendor_id;
        vendor_id.reserve(CPUID_VENDOR_ID_SIZE);

        auto cpu_info = cpuid(0);

        vendor_id.append(std::bit_cast<char*>(&cpu_info.ebx), 4);
        vendor_id.append(std::bit_cast<char*>(&cpu_info.ecx), 4);
        vendor_id.append(std::bit_cast<char*>(&cpu_info.edx), 4);

        return vendor_id;
    }

    auto get_cpu_brand() -> std::string {
        CpuidInfo cpu_info {};

        auto brand_begin = std::bit_cast<char*>(&cpu_info);
        auto brand_end   = brand_begin + sizeof(cpu_info);

        std::string brand(CPUID_BRAND_SIZE, '\0');

        cpu_info = cpuid(0x80000002);
        brand.append(brand_begin, brand_end);

        cpu_info = cpuid(0x80000003);
        brand.append(brand_begin, brand_end);

        cpu_info = cpuid(0x80000004);
        brand.append(brand_begin, brand_end);

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
