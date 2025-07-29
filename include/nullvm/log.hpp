// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// Logging templates.

#ifndef NULLVM_LOG_HPP
#define NULLVM_LOG_HPP

#include <boost/date_time/posix_time/posix_time.hpp>
#include <format>
#include <print>

namespace nullvm::log {

    /// @brief Logs a custom message with a specified format and arguments.
    ///
    /// @param [in] level given custom log level string.
    /// @param [in] fmt given format string.
    /// @param [in] args given format string arguments.
    template<typename... Args>
    auto custom(const std::string_view& level, std::format_string<Args...> fmt, Args&&... args) -> void {
        const auto now = boost::posix_time::second_clock::local_time();
        auto current_time = boost::posix_time::to_simple_string(now);

        std::print("[{}] [{}] ", current_time, level);
        std::println(fmt, std::forward<Args>(args)...);
    }

    /// @brief Logs an informational message.
    ///
    /// @param [in] fmt given format string.
    /// @param [in] args given format string arguments.
    template<typename... Args>
    void info(std::format_string<Args...> fmt, Args&&... args) {
        custom("INFO", fmt, std::forward<Args>(args)...);
    }

    /// @brief Logs a debug message.
    ///
    /// @param [in] fmt given format string.
    /// @param [in] args given format string arguments.
    template<typename... Args>
    void debug(std::format_string<Args...> fmt, Args&&... args) {
        custom("DEBUG", fmt, std::forward<Args>(args)...);
    }

    /// @brief Logs an error message.
    ///
    /// @param [in] fmt given format string.
    /// @param [in] args given format string arguments.
    template<typename... Args>
    void error(std::format_string<Args...> fmt, Args&&... args) {
        custom("ERROR", fmt, std::forward<Args>(args)...);
    }

    /// @brief Logs a message inside tests.
    ///
    /// @param [in] fmt given format string.
    /// @param [in] args given format string arguments.
    template<typename... Args>
    void test(std::format_string<Args...> fmt, Args&&... args) {
        custom("TEST", fmt, std::forward<Args>(args)...);
    }

}

#endif // NULLVM_LOG_HPP
