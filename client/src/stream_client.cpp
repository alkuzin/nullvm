// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// Stream unix domain socket (UDS) client related declarations.

#include <nullvm/service/stream_server.hpp>
#include <nullvm/client/stream_client.hpp>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <cstring>
#include <bit>

namespace nullvm::client {
    using service::STREAM_SERVER_PATH;

    namespace {
        /// Received data buffer size limit in bytes.
        constexpr auto BUFFER_SIZE {256};
    }

    auto StreamClient::init() noexcept -> VmmResult<None> {
        auto sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

        if (sockfd == -1)
            return std::unexpected("Error to create new socket");

        sockaddr_un addr {};

        addr.sun_family = AF_UNIX;
        std::strncpy(
            addr.sun_path, STREAM_SERVER_PATH, sizeof(addr.sun_path) - 1
        );

        m_sockfd = FDWrapper(sockfd);
        m_addr   = addr;

        if (auto result = conn(); !result)
            return result;

        return None {};
    }

    auto StreamClient::conn() noexcept -> VmmResult<None> {
        auto addr = std::bit_cast<sockaddr*>(&m_addr);
        auto ret = connect(m_sockfd.fd(), addr, sizeof(sockaddr_un));

        if (ret == -1)
            return std::unexpected("Error to connect to the server");

        return None {};
    }

    auto StreamClient::send(i32 fd, const Bytes& data) noexcept
    -> VmmResult<None> {
        if (auto ret = write(fd, data.data(), data.size()); ret == -1)
            return std::unexpected("Error to send data to the server");

        return None {};
    }

    auto StreamClient::recv(i32 fd) noexcept -> VmmResult<Bytes> {
        Bytes data {BUFFER_SIZE};

        if (auto ret = read(fd, data.data(), data.size()); ret == -1)
            return std::unexpected("Error to receive data from server");

        return data;
    }

    auto StreamClient::run() noexcept -> VmmResult<None> {
        for (;;) {
            // TODO:
        }
    }

}
