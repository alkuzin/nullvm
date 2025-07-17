// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm hypervisor project and contributors

//! Logging macros.

/// Custom log output.
///
/// # Parameters
/// - `title` - given custom log title.
#[macro_export]
macro_rules! custom {
    ($title:expr, $($arg:tt)*) => {{
        let local_time = chrono::Local::now();
        let timestamp  = local_time.format("%Y-%m-%d %H:%M:%S").to_string();

        print!("[{}] [{}]: ", timestamp, $title);
        print!("{}\n", format_args!($($arg)*));
    }};
}

/// Informational log output.
#[macro_export]
macro_rules! info {
    ($($arg:tt)*) => {{
        $crate::log::custom!("INFO", $($arg)*)
    }};
}

/// Debug log output.
#[macro_export]
macro_rules! debug {
    ($($arg:tt)*) => {{
        #[cfg(debug_assertions)]
        $crate::log::custom!("DEBUG", $($arg)*)
    }};
}

/// Error log output.
#[macro_export]
macro_rules! error {
    ($($arg:tt)*) => {{
        $crate::log::custom!("ERROR", $($arg)*)
    }};
}

/// Test log output.
#[macro_export]
macro_rules! test {
    ($($arg:tt)*) => {{
        $crate::log::custom!("TEST", $($arg)*)
    }};
}

// Re-export macro rules.
pub use custom;
pub use debug;
pub use error;
pub use info;
pub use test;
