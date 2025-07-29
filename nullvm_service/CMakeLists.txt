# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright (C) 2025-present nullvm project and contributors

# CMake configuration file for NullVM service.

cmake_minimum_required(VERSION 3.30.0)
project(nullvm_service)

# List of all source files.
set(SOURCE_FILES src/main.cpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Add include directories to service.
target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

# Link Boost to executable.
target_link_libraries(${PROJECT_NAME} Boost::system)