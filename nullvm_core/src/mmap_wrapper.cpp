// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// Value wrapper for mmap syscall related declarations.

#include <nullvm/core/mmap_wrapper.hpp>
#include <nullvm/log.hpp>
#include <sys/mman.h>

namespace nullvm::core {

    auto MMapWrapper::init(void *addr, usize size) noexcept -> VmmResult<None> {

        if (!addr)
            return std::unexpected("Mapped memory address cannot be null");

        if (addr == MAP_FAILED)
            return std::unexpected("Incorrect memory address: MAP_FAILED");

        if (size == 0)
            return std::unexpected("Mapped data size cannot be 0");

        this->addr = addr;
        this->size = size;

        return None {};
    }

    MMapWrapper::~MMapWrapper() noexcept {

        if (const auto ret = munmap(this->addr, this->size); ret == -1) {
            log::error("Error unmapping memory: {}", std::strerror(errno));
            log::error(
                "munmap: addr: <{:p}> size: {} bytes",
                this->addr, this->size
            );
        }

        this->addr = nullptr;
        this->size = 0;
    }

}
