// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// Unix domain socket (UDS) server related declarations.

#include <nullvm/service/server_uds.hpp>
#include <nullvm/service/stream_uds.hpp>
#include <nullvm/log.hpp>

namespace nullvm::service {

    ServerUDS::ServerUDS(UDSType type) noexcept {
        log::info("Creating UDS server");

        switch (type) {
        case UDSType::Stream:
            log::info("Server type is stream UDS");
            m_inner = std::make_unique<StreamUDS>();
            break;

        case UDSType::Datagram:
            log::info("Server type is datagram UDS");
            // TODO: m_inner = std::make_unique(...);
            break;
        }

        if (!m_inner)
            log::panic("Error to create UDS server");
    }

    auto ServerUDS::init() noexcept -> VmmResult<None> {
        log::info("Initializing UDS server");
        return m_inner->init();
    }

    auto ServerUDS::send(i32 fd, const Bytes& data) noexcept
    -> VmmResult<None> {
        log::debug("Sending {} bytes of data to client {}", data.size(), fd);
        return m_inner->send(fd, data);
    }

    auto ServerUDS::recv(i32 fd) noexcept -> VmmResult<Bytes> {
        auto result = m_inner->recv(fd);

        if (result) {
            const auto& data = result.value();
            log::debug(
                "Received {} bytes of data from client {}", data.size(), fd
            );
        }

        return result;
    }

    auto ServerUDS::run() noexcept -> VmmResult<None> {
        log::info("Running UDS server");

        if (m_inner)
            return m_inner->run();

        return None {};
    }

}
