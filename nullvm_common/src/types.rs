// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm hypervisor project and contributors

//! Virtual machine monitor common types declarations module.

use crate::VmmError;

/// VMM Result type wrapper.
pub type VmmResult<T> = Result<T, VmmError>;
