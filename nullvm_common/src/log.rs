// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm hypervisor project and contributors

//! Logging related declaration.

use crate::VmmResult;
use std::{
    fs::{File, OpenOptions},
    sync::Mutex,
};

/// Logger info struct.
pub struct Logger {
    /// File where logs are stored.
    pub file: File,
}

impl Logger {
    /// Construct new logger.
    ///
    /// # Returns
    /// - New `Logger` object - in case of success.
    /// - `Err` - otherwise.
    pub fn new(path: &str) -> VmmResult<Self> {
        let file = OpenOptions::new().create(true).append(true).open(path)?;
        Ok(Self { file })
    }
}

lazy_static::lazy_static! {
    /// Global logger.
    pub static ref LOGGER: Mutex<Option<Logger>> = Mutex::new(None);
}

/// Initialize logger.
///
/// # Parameters
/// - `path` - given log file path.
#[macro_export]
macro_rules! logger {
    ($path:expr) => {{
        let logger = Logger::new($path).expect("Error to initialize logger");
        let mut log_guard = LOGGER.lock().expect("Error to lock logger");

        let mut file = &logger.file;
        *log_guard = Some(logger);
    }

    $crate::log::info!("------ BEGIN LOGGING ------");};
}

/// Custom log output.
///
/// # Parameters
/// - `title` - given custom log title.
#[macro_export]
macro_rules! custom {
    ($title:expr, $($arg:tt)*) => {{
        use std::io::Write;
        let local_time = chrono::Local::now();
        let timestamp  = local_time.format("%Y-%m-%d %H:%M:%S").to_string();

         if let Some(ref logger) = *$crate::log::LOGGER.lock().unwrap() {
            let mut file = &logger.file;

            writeln!(
                file, "[{}] [{}]: {}", timestamp, $title, format_args!($($arg)*)
            ).expect("Failed to write to log file");
        }
        else {
            eprintln!("Logger is not initialized");
        }
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
pub use logger;
pub use test;
