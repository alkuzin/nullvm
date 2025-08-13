# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright (C) 2025-present nullvm project and contributors

# Main CMake configuration file.

cmake_minimum_required(VERSION 3.30.0)
project(nullvm
        VERSION 0.0.1
        DESCRIPTION "KVM-based type 2 hypervisor"
)

# Macros for project details.
add_definitions(-DPROJECT_NAME="${PROJECT_NAME}")
add_definitions(-DPROJECT_VERSION="${PROJECT_VERSION}")
add_definitions(-DPROJECT_DESCRIPTION="${PROJECT_DESCRIPTION}")

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

# Find GTest.
find_package(GTest REQUIRED)

if(GTest_FOUND)
    include_directories(${GTest_INCLUDE_DIRS})
else()
    message(FATAL_ERROR "GTest library not found")
endif()

enable_testing()

# Compiler flags for different build types.
set(COMMON_CXX_FLAGS "-Wall -Wextra -Werror -Wpedantic -Wconversion")
set(CMAKE_CXX_FLAGS_DEBUG "${COMMON_CXX_FLAGS} -g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${COMMON_CXX_FLAGS} -O3")

# Project subdirectories.
add_subdirectory(core)
add_subdirectory(service)
