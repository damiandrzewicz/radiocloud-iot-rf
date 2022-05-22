#pragma once

#include <unity.h>
#include "utils/MessageBuffer.hpp"
#include "message/RadioMessage.hpp"
#include "message/models/RadioMessageModel.hpp"

namespace test_RadioMessage
{
    MessageBuffer buffer_;

    void test_RadioMessage_createObject()
    {
        // given
        auto type = RadioMessage::Type::Pair;
        auto dir = RadioMessage::Direction::Reqeust;
        RadioMessage rm(buffer_, { type, dir });

        // when

        // then
        TEST_ASSERT_EQUAL(type, rm.metadata().type());
        TEST_ASSERT_EQUAL(dir, rm.metadata().direction());
    }

    void test_RadioMessage_parse_metadataMismatchErr1()
    {
        // given
        auto type = RadioMessage::Type::Pair;
        auto dir = RadioMessage::Direction::Reqeust;
        RadioMessage rm(buffer_, { type, dir });

        // when
        buffer_ = "1|2";
        TEST_ASSERT_FALSE(rm.parse());

        // then
        TEST_ASSERT_EQUAL(RadioMessage::ProcessError::MetadataMismatch, rm.lastProcessError());
    }

    void test_RadioMessage_parse_metadataMismatchErr2()
    {
        // given
        auto type = RadioMessage::Type::Pair;
        auto dir = RadioMessage::Direction::Reqeust;
        RadioMessage rm(buffer_, { type, dir });

        // when
        buffer_ = "1|";
        TEST_ASSERT_FALSE(rm.parse());

        // then
        TEST_ASSERT_EQUAL(RadioMessage::ProcessError::MetadataMismatch, rm.lastProcessError());
    }

        void test_RadioMessage_parse_metadataDoNotVerify()
    {
        // given
        auto type = RadioMessage::Type::Pair;
        auto dir = RadioMessage::Direction::Reqeust;
        RadioMessage rm(buffer_, { type, dir });

        // when
        buffer_ = "1|2";
        TEST_ASSERT_TRUE(rm.parse(false));

        // then
        TEST_ASSERT_EQUAL(RadioMessage::Type::Pair, rm.metadata().type());
        TEST_ASSERT_EQUAL(RadioMessage::Direction::Response, rm.metadata().direction());
    }

    void test_RadioMessage_parse_emptyBufferErr()
    {
        // given
        auto type = RadioMessage::Type::Pair;
        auto dir = RadioMessage::Direction::Reqeust;
        RadioMessage rm(buffer_, { type, dir });

        // when
        buffer_ = "";
        TEST_ASSERT_FALSE(rm.parse());

        // then
        TEST_ASSERT_EQUAL(RadioMessage::ProcessError::EmptyBuffer, rm.lastProcessError());
    }

    void test_RadioMessage_parse_ok()
    {
        // given
        auto type = RadioMessage::Type::Pair;
        auto dir = RadioMessage::Direction::Reqeust;
        RadioMessage rm(buffer_, { type, dir });

        // when
        buffer_ = "1|1";
        TEST_ASSERT_TRUE(rm.parse());
    }

    void test_RadioMessage_build_ok()
    {
        // given
        auto type = RadioMessage::Type::Pair;
        auto dir = RadioMessage::Direction::Reqeust;
        RadioMessage rm(buffer_, { type, dir });

        // when
        TEST_ASSERT_TRUE(rm.build());
        TEST_ASSERT_EQUAL_STRING("1|1", buffer_.data());
    }

    void process()
    {
        RUN_TEST(test_RadioMessage_createObject);
        RUN_TEST(test_RadioMessage_parse_metadataMismatchErr1);
        RUN_TEST(test_RadioMessage_parse_metadataMismatchErr2);
        RUN_TEST(test_RadioMessage_parse_metadataDoNotVerify);
        RUN_TEST(test_RadioMessage_parse_emptyBufferErr);
        RUN_TEST(test_RadioMessage_build_ok);
    }
};