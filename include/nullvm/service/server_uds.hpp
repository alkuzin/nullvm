// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// Unix domain socket (UDS) server related declarations.

#ifndef NULLVM_SERVICE_SERVER_UDS_HPP
#define NULLVM_SERVICE_SERVER_UDS_HPP

#include <nullvm/service/server.hpp>
#include <nullvm/types.hpp>
#include <memory>

namespace nullvm::service {

    /// UDS servers type enumeration.
    enum class UDSType: u8 {
        /// Stream UDS server type.
        Stream,
        /// Datagram UDS server type.
        Datagram
    };

    /// UDS server class.
    class ServerUDS final : public Server {
        /// Inner implementation of UDS server.
        std::unique_ptr<Server> m_inner;

    public:
        /// @brief Construct new ServerUDS object.
        ///
        /// @param [in] type given UDS server type.
        ServerUDS(UDSType type) noexcept;
    };

}

#endif // NULLVM_SERVICE_SERVER_UDS_HPP
