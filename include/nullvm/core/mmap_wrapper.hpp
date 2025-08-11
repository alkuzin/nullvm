// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// Value wrapper for mmap syscall related declarations.

#ifndef NULLVM_CORE_MMAP_WRAPPER_HPP
#define NULLVM_CORE_MMAP_WRAPPER_HPP

#include <nullvm/types.hpp>
#include <memory>

namespace nullvm::core {

    /// Mapped data deleter struct.
    struct MMapDeleter final {
        /// Mapped data size in bytes.
        usize m_size;

        /// @brief Construct new MMapDeleter object.
        ///
        /// @param [in] size given mapped data size in bytes.
        MMapDeleter(usize size) noexcept : m_size(size) {}

        /// @brief Unmap mapped memory address.
        ///
        /// @param [in] addr given mapped data memory address.
        auto operator()(void *addr) const noexcept -> void;
    };

    /// Value wrapper for mmap syscall.
    class MMapWrapper final {
        /// Mapped data memory address.
        std::unique_ptr<void, MMapDeleter> m_addr;
        /// Mapped data size in bytes.
        usize m_size;

    public:
        /// @brief Construct new MMapWrapper object.
        MMapWrapper() noexcept: m_addr(nullptr, 0), m_size(0) {}

        /// @brief Initialize MMapWrapper object.
        ///
        /// @param [in] addr given mapped data memory address.
        /// @param [in] size given mapped data size in bytes.
        ///
        /// @return None - in case of success.
        /// @return VmmError - otherwise.
        auto init(void *addr, usize size) noexcept -> VmmResult<None>;

        /// @brief Get mapped data memory address.
        ///
        /// @return Mapped data memory address.
        auto addr() const noexcept -> void *;

        /// @brief Get mapped data size in bytes.
        ///
        /// @return Mapped data size in bytes.
        auto size() const noexcept -> usize;
    };

}

#endif // NULLVM_CORE_MMAP_WRAPPER_HPP
