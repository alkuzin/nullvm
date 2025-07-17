// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm hypervisor project and contributors

//! x86 CPU related declarations module.

use std::arch::x86_64::__cpuid;

/// CPU vendor identifier size.
const VENDOR_ID_SIZE: usize = 32;

/// CPU brand size.
const BRAND_SIZE: usize = 64;

/// Get CPU vendor info.
///
/// # Returns
/// - CPU vendor string representation.
pub fn get_cpu_vendor() -> String {
    let mut vendor_id: Vec<u8> = Vec::with_capacity(VENDOR_ID_SIZE);
    let cpu_info = unsafe { __cpuid(0) };

    // Extracting vendor ID assuming that byte order is little endian.
    vendor_id.extend_from_slice(&cpu_info.ebx.to_le_bytes());
    vendor_id.extend_from_slice(&cpu_info.edx.to_le_bytes());
    vendor_id.extend_from_slice(&cpu_info.ecx.to_le_bytes());

    String::from_utf8(vendor_id).unwrap_or("Unknown".to_string())
}

/// Get CPU brand.
///
/// # Returns
/// - CPU brand string representation.
pub fn get_cpuid_brand() -> String {
    let mut brand: Vec<u8> = vec![0; BRAND_SIZE];

    // Retrieve the three parts of the CPU brand.
    for leaf in 0x80000002..=0x80000004 {
        let cpu_info = unsafe { __cpuid(leaf) };
        let part = [cpu_info.eax, cpu_info.ebx, cpu_info.ecx, cpu_info.edx];

        // Copy the data into the brand array.
        let start_index = (leaf - 0x80000002) * 16;

        for (i, &value) in part.iter().enumerate() {
            let offset = start_index as usize + i * 4;
            let value = &value as *const u32 as *const u8;

            unsafe {
                std::ptr::copy_nonoverlapping(value, &mut brand[offset], 4);
            }
        }
    }

    String::from_utf8(brand).unwrap_or("Unknown".to_string())
}

/// Check whether CPU supporting virtualization.
///
/// # Returns
/// - `true`  - if CPU supporting virtualization.
/// - `false` - otherwise.
pub fn is_virtualization_supported() -> bool {
    let cpu_info = unsafe { __cpuid(1) };
    (cpu_info.ecx & (1 << 5)) != 0x0
}
