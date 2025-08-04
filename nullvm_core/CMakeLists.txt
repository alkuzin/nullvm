# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright (C) 2025-present nullvm project and contributors

# CMake configuration file for NullVM core library.

cmake_minimum_required(VERSION 3.30.0)
project(nullvm_core)

# List of all source files.
set(SOURCE_FILES
        src/cpu.cpp
        src/vmfd.cpp
        src/kvm.cpp
        src/vm.cpp
)

# Create a shared library.
set(LIBRARY_NAME ${PROJECT_NAME})
add_library(${LIBRARY_NAME} SHARED ${SOURCE_FILES})

# Add include directories to library.
target_include_directories(${LIBRARY_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

# Library tests.
enable_testing()

set(TESTS_EXECUTABLE nullvm_core_tests)
set(TESTS_SOURCE_FILES
        tests/test_vmfd.cpp
        tests/test_kvm.cpp
        tests/test_vm.cpp
)

add_executable(${TESTS_EXECUTABLE} ${TESTS_SOURCE_FILES})

# Link the GTest library to the test project.
target_link_libraries(${TESTS_EXECUTABLE} PRIVATE ${LIBRARY_NAME}
        GTest::Main GTest::GTest
)

# Add include directories to tests.
target_include_directories(${TESTS_EXECUTABLE} PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

add_test(NAME nullvm_core_tests COMMAND ${TESTS_EXECUTABLE})