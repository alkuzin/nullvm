// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// Stream unix domain socket (UDS) server tests.

#include <nullvm/service/stream_uds.hpp>
#include <gtest/gtest.h>
#include <unistd.h>
#include <fcntl.h>

using namespace nullvm::service;

TEST(test_stream_uds, test_stream_uds_initialization) {
    auto server = StreamUDS();
    auto result = server.init();

    EXPECT_TRUE(result.has_value());
}

TEST(test_stream_uds, test_stream_uds_send_data_correct) {
    auto server = StreamUDS();
    auto result = server.init();
    EXPECT_TRUE(result.has_value());

    auto fd = open("/dev/null", O_WRONLY);
    auto data = {std::byte{0x01}, std::byte{0x02}, std::byte{0x03}};

    result = server.send(fd, data);
    EXPECT_TRUE(result.has_value());

    close(fd);
}

TEST(test_stream_uds, test_stream_uds_send_data_incorrect) {
    auto server = StreamUDS();
    auto result = server.init();
    EXPECT_TRUE(result.has_value());

    auto data = {std::byte{0x01}, std::byte{0x02}, std::byte{0x03}};

    result = server.send(-1, data);
    EXPECT_FALSE(result.has_value());
}

TEST(test_stream_uds, test_stream_uds_recv_data_correct) {
    auto server = StreamUDS();
    auto result = server.init();
    EXPECT_TRUE(result.has_value());

    auto fd = open("/dev/zero", O_RDONLY);
    auto recv_result = server.recv(fd);
    EXPECT_TRUE(recv_result.has_value());

    close(fd);
}

TEST(test_stream_uds, test_stream_uds_recv_data_incorrect) {
    auto server = StreamUDS();
    auto result = server.init();
    EXPECT_TRUE(result.has_value());

    auto recv_result = server.recv(-1);
    EXPECT_FALSE(recv_result.has_value());
}