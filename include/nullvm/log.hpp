// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// Logging templates.

#ifndef NULLVM_LOG_HPP
#define NULLVM_LOG_HPP

#include <boost/date_time/posix_time/posix_time.hpp>
#include <source_location>
#include <string_view>
#include <format>
#include <print>

namespace nullvm::log {

    /// @brief Logs a custom message into string.
    ///
    /// @param [in] level given custom log level string.
    /// @param [in] fmt given format string.
    /// @param [in] args given format string arguments.
    ///
    /// @return Custom message log string.
    template<typename... Args>
    auto custom_format(
        const std::string_view& level,
        std::format_string<Args...> fmt,
        Args&&... args
    )
    -> std::string {

        const auto now = boost::posix_time::second_clock::local_time();
        auto current_time = boost::posix_time::to_simple_string(now);

        auto time_str = std::format("[{}] [{}] ", current_time, level);
        auto log_str = std::format(fmt, std::forward<Args>(args)...);

        return time_str + log_str + "\n";
    }

    /// @brief Logs a custom message with a specified format and arguments.
    ///
    /// @param [in] level given custom log level string.
    /// @param [in] fmt given format string.
    /// @param [in] args given format string arguments.
    template<typename... Args>
    auto custom(
        const std::string_view& level,
        std::format_string<Args...> fmt,
        Args&&... args
    )
    -> void {
        auto msg = custom_format(level, fmt, std::forward<Args>(args)...);
        std::print("{}", msg);
    }

    /// @brief Logs an informational message.
    ///
    /// @param [in] fmt given format string.
    /// @param [in] args given format string arguments.
    template<typename... Args>
    auto info(std::format_string<Args...> fmt, Args&&... args) -> void {
        custom("INFO", fmt, std::forward<Args>(args)...);
    }

    /// @brief Logs a debug message.
    ///
    /// @param [in] fmt given format string.
    /// @param [in] args given format string arguments.
    template<typename... Args>
    auto debug(std::format_string<Args...> fmt, Args&&... args) -> void {
        custom("DEBUG", fmt, std::forward<Args>(args)...);
    }

    /// @brief Logs an error message.
    ///
    /// @param [in] fmt given format string.
    /// @param [in] args given format string arguments.
    template<typename... Args>
    auto error(std::format_string<Args...> fmt, Args&&... args) -> void {
        custom("ERROR", fmt, std::forward<Args>(args)...);
    }

    /// @brief Logs a message inside tests.
    ///
    /// @param [in] fmt given format string.
    /// @param [in] args given format string arguments.
    template<typename... Args>
    auto test(std::format_string<Args...> fmt, Args&&... args) -> void {
        custom("TEST", fmt, std::forward<Args>(args)...);
    }

    /// Panic format string wrapper.
    template <typename... Args>
    struct PanicFormat {
        /// Log format string.
        std::format_string<Args...> format;
        /// Source code location info where panic occured.
        std::source_location location;

        /// @brief Construct new PanicFormat object.
        ///
        /// @param str given format string.
        /// @param loc given source code location info.
        template <typename T>
        consteval PanicFormat(
            const T &str,
            const std::source_location loc = std::source_location::current()
        ) noexcept : format{str}, location{loc} {}
    };

    /// @brief Logs a panic message and terminates process.
    ///
    /// @param [in] fmt given format string.
    /// @param [in] args given format string arguments.
    template<typename... Args>
    [[noreturn]]
    auto panic(
        const PanicFormat<std::type_identity_t<Args>...>& fmt,
        Args&&... args
    ) -> void {

        const auto& loc = fmt.location;

        custom("PANIC", "Panic occured in file '{}'", loc.file_name());
        custom("PANIC", "In function '{}'", loc.function_name());
        custom("PANIC", "On line: {}", loc.line());
        custom("PANIC", fmt.format, std::forward<Args>(args)...);

        std::terminate();
    }

}

#endif // NULLVM_LOG_HPP
