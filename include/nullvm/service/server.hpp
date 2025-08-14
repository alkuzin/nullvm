// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// Server abstract class related declarations.

#ifndef NULLVM_SERVICE_SERVER_HPP
#define NULLVM_SERVICE_SERVER_HPP

#include <nullvm/types.hpp>

namespace nullvm::service {

    /// Server abstract class.
    class Server {
    public:
        /// @brief Destroy Server object.
        virtual ~Server() noexcept = default;

        /// @brief Initialize server.
        ///
        /// @return None - in case of success.
        /// @return VmmError - otherwise.
        virtual auto init() noexcept -> VmmResult<None> = 0;

        /// @brief Send data to client.
        ///
        /// @param [in] fd given client connection file descriptor.
        /// @param [in] data given sequence of bytes to send.
        ///
        /// @return None - in case of success.
        /// @return VmmError - otherwise.
        virtual auto send(i32 fd, const Bytes& data) noexcept
        -> VmmResult<None> = 0;

        /// @brief Receive data from client.
        ///
        /// @param [in] fd given client connection file descriptor.
        ///
        /// @return Received sequence of bytes - in case of success.
        /// @return VmmError - otherwise.
        virtual auto recv(i32 fd) noexcept -> VmmResult<Bytes> = 0;

        /// @brief Run server.
        ///
        /// @return None - in case of success.
        /// @return VmmError - otherwise.
        virtual auto run() noexcept -> VmmResult<None> = 0;
    };

}

#endif // NULLVM_SERVICE_SERVER_HPP
