// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// Unix domain socket (UDS) server related declarations.

#include <nullvm/service/server_uds.hpp>
#include <nullvm/service/stream_uds.hpp>

namespace nullvm::service {

    ServerUDS::ServerUDS(UDSType type) noexcept {
        switch (type) {
        case UDSType::Stream:
            m_inner = std::make_unique<StreamUDS>();
            break;

        case UDSType::Datagram:
            // TODO: m_inner = std::make_unique(...);
            break;
        }
    }

}
