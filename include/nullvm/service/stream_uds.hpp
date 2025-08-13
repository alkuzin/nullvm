// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// Stream unix domain socket (UDS) server related declarations.

#ifndef NULLVM_SERVICE_STREAM_UDS_HPP
#define NULLVM_SERVICE_STREAM_UDS_HPP

#include <nullvm/core/utils/fd_wrapper.hpp>
#include <nullvm/service/server.hpp>
#include <nullvm/types.hpp>
#include <sys/un.h>

namespace nullvm::service {
    using core::utils::FDWrapper;

    /// Stream UDS server socket path.
    /// TODO: change socket directory from '/tmp'.
    constexpr auto STREAM_SERVER_PATH {"/tmp/nullvm_stream_server"};

    /// Stream UDS server connection requests limit.
    constexpr auto STREAM_SERVER_BACKLOG {5};

    /// Stream UDS server class.
    class StreamUDS final : public Server {
        /// Socket file descriptor.
        FDWrapper m_sockfd;
        /// Server address.
        sockaddr_un m_addr;

    public:
        /// @brief Initialize StreamUDS object.
        ///
        /// @return None - in case of success.
        /// @return VmmError - otherwise.
        auto init() noexcept -> VmmResult<None>;

        /// @brief Run server.
        ///
        /// @return None - in case of success.
        /// @return VmmError - otherwise.
        auto run() noexcept -> VmmResult<None>;

    private:
        /// @brief Bind server address.
        ///
        /// @return None - in case of success.
        /// @return VmmError - otherwise.
        auto link() noexcept -> VmmResult<None>;
    };

}

#endif // NULLVM_SERVICE_STREAM_UDS_HPP
