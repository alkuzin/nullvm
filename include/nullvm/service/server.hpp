// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// Server abstract class related declarations.

#ifndef NULLVM_SERVICE_SERVER_HPP
#define NULLVM_SERVICE_SERVER_HPP

namespace nullvm::service {

    /// Server abstract class.
    class Server {
    public:
        /// @brief Destroy Server object.
        virtual ~Server() noexcept = 0;
    };

}

#endif // NULLVM_SERVICE_SERVER_HPP
