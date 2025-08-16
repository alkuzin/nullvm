// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// Stream unix domain socket (UDS) client related declarations.

#ifndef NULLVM_CLIENT_STREAM_CLIENT_HPP
#define NULLVM_CLIENT_STREAM_CLIENT_HPP

#include <nullvm/core/utils/fd_wrapper.hpp>
#include <nullvm/client/client.hpp>
#include <sys/un.h>

namespace nullvm::client {
    using core::utils::FDWrapper;

    /// Stream UDS server class.
    class StreamClient final : public Client {
        /// Socket file descriptor.
        FDWrapper m_sockfd;
        /// Client address.
        sockaddr_un m_addr;

    public:
        /// @brief Initialize StreamClient object.
        ///
        /// @return None - in case of success.
        /// @return VmmError - otherwise.
        auto init() noexcept -> VmmResult<None> override;

        /// @brief Send data to server.
        ///
        /// @param [in] fd given server connection file descriptor.
        /// @param [in] data given sequence of bytes to send.
        ///
        /// @return None - in case of success.
        /// @return VmmError - otherwise.
        auto send(i32 fd, const Bytes& data) noexcept
        -> VmmResult<None> override;

        /// @brief Receive data from server.
        ///
        /// @param [in] fd given server connection file descriptor.
        ///
        /// @return Received sequence of bytes - in case of success.
        /// @return VmmError - otherwise.
        auto recv(i32 fd) noexcept -> VmmResult<Bytes> override;

        /// @brief Run client.
        ///
        /// @return None - in case of success.
        /// @return VmmError - otherwise.
        auto run() noexcept -> VmmResult<None> override;

    private:
        /// @brief Connect to server.
        ///
        /// @return None - in case of success.
        /// @return VmmError - otherwise.
        auto conn() noexcept -> VmmResult<None>;
    };

}

#endif // NULLVM_CLIENT_STREAM_CLIENT_HPP
