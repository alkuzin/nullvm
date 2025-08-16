// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025-present nullvm project and contributors

/// Stream unix domain socket (UDS) client tests.

#include <nullvm/service/stream_server.hpp>
#include <nullvm/client/stream_client.hpp>
#include <gtest/gtest.h>
#include <unistd.h>
#include <fcntl.h>

using namespace nullvm::service;
using namespace nullvm::client;

TEST(test_stream_client, test_stream_client_initialization_correct) {
    auto server = StreamServer();
    auto result = server.init();

    EXPECT_TRUE(result.has_value());

    auto client = StreamClient();
    result = client.init();

    EXPECT_TRUE(result.has_value());
}

TEST(test_stream_client, test_stream_client_initialization_incorrect) {
    auto client = StreamClient();
    auto result = client.init();

    EXPECT_FALSE(result.has_value());
    GTEST_LOG_(INFO) << "Result: " << result.error() << "\n";
}

TEST(test_stream_client, test_stream_client_send_data_correct) {
    auto server = StreamServer();
    auto result = server.init();

    EXPECT_TRUE(result.has_value());

    auto client = StreamClient();
    result = client.init();
    EXPECT_TRUE(result.has_value());

    auto fd = open("/dev/null", O_WRONLY);
    auto data = {std::byte{0x01}, std::byte{0x02}, std::byte{0x03}};

    result = client.send(fd, data);
    EXPECT_TRUE(result.has_value());

    close(fd);
}

TEST(test_stream_client, test_stream_client_send_data_incorrect) {
    auto server = StreamServer();
    auto result = server.init();

    EXPECT_TRUE(result.has_value());

    auto client = StreamClient();
    result = client.init();
    EXPECT_TRUE(result.has_value());

    auto data = {std::byte{0x01}, std::byte{0x02}, std::byte{0x03}};

    result = client.send(-1, data);
    EXPECT_FALSE(result.has_value());
}

TEST(test_stream_client, test_stream_client_recv_data_correct) {
    auto server = StreamServer();
    auto result = server.init();

    EXPECT_TRUE(result.has_value());

    auto client = StreamClient();
    result = client.init();
    EXPECT_TRUE(result.has_value());

    auto fd = open("/dev/zero", O_RDONLY);
    auto recv_result = client.recv(fd);
    EXPECT_TRUE(recv_result.has_value());

    close(fd);
}

TEST(test_stream_client, test_stream_client_recv_data_incorrect) {
    auto server = StreamServer();
    auto result = server.init();

    EXPECT_TRUE(result.has_value());

    auto client = StreamClient();
    result = client.init();
    EXPECT_TRUE(result.has_value());

    auto recv_result = client.recv(-1);
    EXPECT_FALSE(recv_result.has_value());
}