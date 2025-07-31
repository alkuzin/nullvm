// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// Common project types.

#ifndef NULLVM_TYPES_HPP
#define NULLVM_TYPES_HPP

#include <expected>
#include <variant>
#include <cstdint>
#include <string>

namespace nullvm {

    // Unsigned types aliases.
    using u64 = std::uint64_t;
    using u32 = std::uint32_t;
    using u16 = std::uint32_t;
    using u8  = std::uint8_t;

    // Signed types aliases.
    using i64 = std::int64_t;
    using i32 = std::int32_t;
    using i16 = std::int32_t;
    using i8  = std::int8_t;

    // Floating point types aliases.
    using f64 = double;
    using f32 = float;

    // Size types aliases.
    using usize = std::size_t;

    /// VMM error type.
    using VmmError = std::string;

    /// VMM expected values wrapper.
    template <typename T>
    using VmmResult = std::expected<T, VmmError>;

    /// Placeholder type for VmmResult.
    using None = std::monostate;

}

#endif // NULLVM_TYPES_HPP
