# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright (C) 2025-present nullvm project and contributors

# CMake configuration file for NullVM service.

cmake_minimum_required(VERSION 3.30.0)
project(nullvm_service)

# List of all source files.
set(SOURCE_FILES
        src/server_uds.cpp
        src/stream_server.cpp
)

# Create a shared library.
set(LIBRARY_NAME ${PROJECT_NAME}_lib)
add_library(${LIBRARY_NAME} SHARED ${SOURCE_FILES})

# Create a main executable.
add_executable(${PROJECT_NAME} src/main.cpp)

# Link the service library to the main executable.
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBRARY_NAME})

# Add include directories to service library.
target_include_directories(${LIBRARY_NAME} PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

# Link Boost to executable.
target_link_libraries(${LIBRARY_NAME} Boost::system nullvm_core)

# Library tests.
enable_testing()

set(TESTS_EXECUTABLE nullvm_service_tests)
set(TESTS_SOURCE_FILES
        tests/test_stream_server.cpp
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

add_test(NAME nullvm_service_tests COMMAND ${TESTS_EXECUTABLE})