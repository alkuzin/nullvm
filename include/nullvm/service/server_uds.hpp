// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// Unix domain socket (UDS) server related declarations.

#ifndef NULLVM_SERVICE_SERVER_UDS_HPP
#define NULLVM_SERVICE_SERVER_UDS_HPP

#include <nullvm/service/server.hpp>
#include <nullvm/types.hpp>
#include <memory>

namespace nullvm::service {

    /// UDS server class.
    class ServerUDS final : public Server {
        /// Inner implementation of UDS server.
        std::unique_ptr<Server> m_inner;

    public:
        /// @brief Construct new ServerUDS object.
        ///
        /// @param [in] type given UDS server type.
        ServerUDS(UDSType type) noexcept;

        /// @brief Initialize ServerUDS object.
        ///
        /// @return None - in case of success.
        /// @return VmmError - otherwise.
        auto init() noexcept -> VmmResult<None> override;

        /// @brief Send data to client.
        ///
        /// @param [in] fd given client connection file descriptor.
        /// @param [in] data given sequence of bytes to send.
        ///
        /// @return None - in case of success.
        /// @return VmmError - otherwise.
        auto send(i32 fd, const Bytes& data) noexcept
        -> VmmResult<None> override;

        /// @brief Receive data from client.
        ///
        /// @param [in] fd given client connection file descriptor.
        ///
        /// @return Received sequence of bytes - in case of success.
        /// @return VmmError - otherwise.
        auto recv(i32 fd) noexcept -> VmmResult<Bytes> override;

        /// @brief Run server.
        ///
        /// @return None - in case of success.
        /// @return VmmError - otherwise.
        auto run() noexcept -> VmmResult<None> override;
    };

}

#endif // NULLVM_SERVICE_SERVER_UDS_HPP
