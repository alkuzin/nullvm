// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// Value wrapper for mmap syscall related declarations.

#include <nullvm/core/mmap_wrapper.hpp>
#include <nullvm/log.hpp>
#include <sys/mman.h>

namespace nullvm::core {

    auto MMapDeleter::operator()(void *addr) const noexcept -> void {
        if (m_size != 0) {
            if (const auto ret = munmap(addr, m_size); ret == -1)
                log::panic("Error unmapping memory: {}", std::strerror(errno));
        }
    }

    auto MMapWrapper::init(void *addr, usize size) noexcept -> VmmResult<None> {

        if (!addr)
            return std::unexpected("Mapped memory address cannot be null");

        if (addr == MAP_FAILED)
            return std::unexpected("Incorrect memory address: MAP_FAILED");

        if (size == 0)
            return std::unexpected("Mapped data size cannot be 0");

        m_addr = std::unique_ptr<void, MMapDeleter>(addr, size);
        m_size = size;

        return None {};
    }

    auto MMapWrapper::addr() const noexcept -> void * {
        return m_addr.get();
    }

    auto MMapWrapper::size() const noexcept -> usize {
        return m_size;
    }

}
