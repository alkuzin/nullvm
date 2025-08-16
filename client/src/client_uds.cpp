// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// Unix domain socket (UDS) client related declarations.

#include <nullvm/client/stream_client.hpp>
#include <nullvm/client/client_uds.hpp>
#include <nullvm/log.hpp>

namespace nullvm::client {

    ClientUDS::ClientUDS(UDSType type) noexcept {
        log::info("Creating UDS client");

        switch (type) {
        case UDSType::Stream:
            log::info("Client type is stream UDS");
            m_inner = std::make_unique<StreamClient>();
            break;

        case UDSType::Datagram:
            log::info("Client type is datagram UDS");
            // TODO: m_inner = std::make_unique(...);
            break;
        }

        if (!m_inner)
            log::panic("Error to create UDS client");
    }

    auto ClientUDS::init() noexcept -> VmmResult<None> {
        log::info("Initializing UDS client");
        return m_inner->init();
    }

    auto ClientUDS::send(i32 fd, const Bytes& data) noexcept
    -> VmmResult<None> {
        log::debug("Sending {} bytes of data to server {}", data.size(), fd);
        return m_inner->send(fd, data);
    }

    auto ClientUDS::recv(i32 fd) noexcept -> VmmResult<Bytes> {
        auto result = m_inner->recv(fd);

        if (result) {
            const auto& data = result.value();
            log::debug(
                "Received {} bytes of data from server {}", data.size(), fd
            );
        }

        return result;
    }

    auto ClientUDS::run() noexcept -> VmmResult<None> {
        log::info("Running UDS client");

        if (m_inner)
            return m_inner->run();

        return None {};
    }

}
