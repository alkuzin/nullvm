// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// Client abstract class related declarations.

#ifndef NULLVM_CLIENT_CLIENT_HPP
#define NULLVM_CLIENT_CLIENT_HPP

#include <nullvm/types.hpp>

namespace nullvm::client {

    /// Client abstract class.
    class Client {
    public:
        /// @brief Destroy Client object.
        virtual ~Client() noexcept = default;

        /// @brief Initialize client.
        ///
        /// @return None - in case of success.
        /// @return VmmError - otherwise.
        virtual auto init() noexcept -> VmmResult<None> = 0;

        /// @brief Send data to server.
        ///
        /// @param [in] fd given server connection file descriptor.
        /// @param [in] data given sequence of bytes to send.
        ///
        /// @return None - in case of success.
        /// @return VmmError - otherwise.
        virtual auto send(i32 fd, const Bytes& data) noexcept
        -> VmmResult<None> = 0;

        /// @brief Receive data from server.
        ///
        /// @param [in] fd given server connection file descriptor.
        ///
        /// @return Received sequence of bytes - in case of success.
        /// @return VmmError - otherwise.
        virtual auto recv(i32 fd) noexcept -> VmmResult<Bytes> = 0;

        /// @brief Run client.
        ///
        /// @return None - in case of success.
        /// @return VmmError - otherwise.
        virtual auto run() noexcept -> VmmResult<None> = 0;
    };

}

#endif // NULLVM_CLIENT_CLIENT_HPP
