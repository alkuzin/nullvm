# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright (C) 2025-present nullvm project and contributors

# Main CMake configuration file.

cmake_minimum_required(VERSION 3.30.0)
project(nullvm)

# Set the C++ compiler to Clang.
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

# Use C++23 standard.
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set default build type.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Find Boost libraries.
find_package(Boost REQUIRED COMPONENTS system unit_test_framework)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
else()
    message(FATAL_ERROR "Boost libraries not found")
endif()

# Compiler flags for different build types.
set(COMMON_CXX_FLAGS "-Wall -Wextra -Werror -Wpedantic -Wconversion")
set(CMAKE_CXX_FLAGS_DEBUG "${COMMON_CXX_FLAGS} -g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${COMMON_CXX_FLAGS} -O3")

enable_testing()

# Project subdirectories.
add_subdirectory(nullvm_core)
add_subdirectory(nullvm_service)
