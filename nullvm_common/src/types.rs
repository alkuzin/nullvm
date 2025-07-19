// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm hypervisor project and contributors

//! Virtual machine monitor common types declarations module.

use crate::VmmError;
use std::{
    ffi::c_void,
    mem::ManuallyDrop,
    ops::{Deref, DerefMut},
};

/// VMM Result type wrapper.
pub type VmmResult<T> = Result<T, VmmError>;

/// Memory map syscall value wrapper.
pub type MMapWrapper = ManualFree<*mut c_void, Box<dyn FnOnce(*mut c_void)>>;

/// Value wrapper with manual cleanup.
pub struct ManualFree<T, F: FnOnce(T)> {
    /// Allocated value.
    value: ManuallyDrop<T>,
    /// Value cleanup procedure.
    cleanup: ManuallyDrop<F>,
}

impl<T, F: FnOnce(T)> ManualFree<T, F> {
    /// Construct new `ManualFree` wrapper.
    ///
    /// # Parameters
    /// - `value` - given value to wrap.
    /// - `cleanup` - given value cleanup procedure.
    ///
    /// # Returns
    /// - New `ManualFree` object.
    pub fn new(value: T, cleanup: F) -> Self {
        Self {
            value: ManuallyDrop::new(value),
            cleanup: ManuallyDrop::new(cleanup),
        }
    }
}

/// Dereference wrapped value.
impl<T, F: FnOnce(T)> Deref for ManualFree<T, F> {
    type Target = T;

    fn deref(&self) -> &Self::Target {
        &self.value
    }
}

/// Mutably dereference wrapped value.
impl<T, F: FnOnce(T)> DerefMut for ManualFree<T, F> {
    fn deref_mut(&mut self) -> &mut Self::Target {
        &mut self.value
    }
}

/// Cleanup wrapped value.
impl<T, F: FnOnce(T)> Drop for ManualFree<T, F> {
    fn drop(&mut self) {
        let cleanup = unsafe { ManuallyDrop::take(&mut self.cleanup) };
        let value = unsafe { ManuallyDrop::take(&mut self.value) };

        cleanup(value);
    }
}

#[cfg(test)]
pub mod tests {
    use super::*;
    use libc::{free, malloc};

    #[test]
    fn test_auto_free() {
        let mut is_cleaned = false;

        let value = unsafe { malloc(16) };
        let cleanup = {
            let is_cleaned_clone = &mut is_cleaned;
            let value_clone = value.clone();

            move |value| {
                assert_eq!(value, value_clone);

                unsafe { free(value) };
                *is_cleaned_clone = true;
            }
        };

        let wrapper = ManualFree::new(value, cleanup);
        drop(wrapper);

        assert!(is_cleaned);
    }

    #[test]
    fn test_get_value_immutable() {
        let value = 42;
        let cleanup = |_| { /* Do nothing. */ };
        let wrapper = ManualFree::new(value, cleanup);

        let result = wrapper.deref();
        assert_eq!(value, *result);
    }

    #[test]
    fn test_get_value_mutable() {
        let value = 42;
        let cleanup = |_| { /* Do nothing. */ };
        let mut wrapper = ManualFree::new(value, cleanup);

        let result;
        {
            result = wrapper.deref_mut();
            *result += 12;
        }

        let value = wrapper.deref();
        assert_eq!(*value, 54);
    }
}
