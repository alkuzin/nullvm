// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// Unix domain socket (UDS) client related declarations.

#ifndef NULLVM_CLIENT_CLIENT_UDS_HPP
#define NULLVM_CLIENT_CLIENT_UDS_HPP

#include <nullvm/client/client.hpp>
#include <nullvm/types.hpp>
#include <memory>

namespace nullvm::client {

    /// UDS server class.
    class ClientUDS final : public Client {
        /// Inner implementation of UDS server.
        std::unique_ptr<Client> m_inner;

    public:
        /// @brief Construct new ClientUDS object.
        ///
        /// @param [in] type given UDS server type.
        ClientUDS(UDSType type) noexcept;

        /// @brief Initialize ClientUDS object.
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
    };

}

#endif // NULLVM_CLIENT_CLIENT_UDS_HPP
