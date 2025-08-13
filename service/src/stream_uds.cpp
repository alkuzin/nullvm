// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// Stream unix domain socket (UDS) server related declarations.

#include <nullvm/service/stream_uds.hpp>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <cstring>
#include <bit>

namespace nullvm::service {

    auto StreamUDS::init() noexcept -> VmmResult<None> {
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

        if (auto ret = remove(STREAM_SERVER_PATH); ret == -1 && errno != ENOENT)
            return std::unexpected("Error to create new socket");

        if (auto result = link(); !result)
            return result;

        if (auto ret = listen(sockfd, STREAM_SERVER_BACKLOG); ret == -1)
            return std::unexpected("Error to listen for new connections");

        return None {};
    }

    auto StreamUDS::link() noexcept -> VmmResult<None> {
        auto addr = std::bit_cast<sockaddr*>(&m_addr);
        auto ret = bind(m_sockfd.fd(), addr, sizeof(sockaddr_un));

        if (ret == -1)
            return std::unexpected("Error to bind server address");

        return None {};
    }

    auto StreamUDS::run() noexcept -> VmmResult<None> {
        for (;;) {
            auto clientfd = accept(m_sockfd.fd(), nullptr, nullptr);

            if (clientfd == -1)
                return std::unexpected("Error to accept client connection");

            // TODO: handle client messages.

            if (auto ret = close(clientfd); ret == -1)
                return std::unexpected("Error to close connection with client");
        }
    }
}
