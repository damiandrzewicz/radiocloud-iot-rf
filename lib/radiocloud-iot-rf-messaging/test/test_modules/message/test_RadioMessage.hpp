#pragma once

#include <unity.h>
#include "utils/MessageBuffer.hpp"
#include "message/RadioMessage.hpp"
#include "message/models/RadioMessageModel.hpp"

namespace test_RadioMessage
{
    MessageBuffer buffer_;

    void test_RadioMessage_build_emptyModel(void) {
        // given
        RadioMessage obj(buffer_);
        RadioMessageModel model;

        // when
        auto res = obj.build(model);

        // then
        TEST_ASSERT_EQUAL(RadioMessageResult::MODEL_MALFORMED, res);
        TEST_ASSERT_FALSE(buffer_.isFilled());
    }

    void test_RadioMessage_build_filledBuffer(void) {
        // given
        buffer_.appendText("someTrash...");
        RadioMessage obj(buffer_);

        RadioMessageModel model;
        model.messageDirection = MessageDirection::Response;
        model.radioMessageType = RadioMessageType::Pair;

        // when
        auto res = obj.build(model);

        // then
        TEST_ASSERT_EQUAL(RadioMessageResult::OK, res);
        TEST_ASSERT_EQUAL_STRING("1|2", buffer_.data());
    }

    void test_RadioMessage_build(void) {
        // given
        RadioMessage obj(buffer_);

        RadioMessageModel model;
        model.messageDirection = MessageDirection::Response;
        model.radioMessageType = RadioMessageType::Pair;

        // when
        auto res = obj.build(model);

        // then
        TEST_ASSERT_EQUAL(RadioMessageResult::OK, res);
        TEST_ASSERT_EQUAL_STRING("1|2", buffer_.data());
    }

    void test_RadioMessage_parse_emptyBuffer(void) {
        // given
        buffer_.clear();
        RadioMessage obj(buffer_);
        RadioMessageModel model;

        // when
        auto res = obj.parse(model);

        // then
        TEST_ASSERT_EQUAL(RadioMessageResult::EMPTY_BUFFER, res);
    }

    void test_RadioMessage_parse_messageMalformed(void) {
        // given
        buffer_ = "|2";
        RadioMessage obj(buffer_);
        RadioMessageModel model;

        // when
        auto res = obj.parse(model);

        // then
        TEST_ASSERT_EQUAL(RadioMessageResult::MESSAGE_MALFORMED, res);
    }

    void test_RadioMessage_parse(void) {
        // given
        buffer_ = "1|2";
        RadioMessage obj(buffer_);
        RadioMessageModel model;
        model.messageDirection = MessageDirection::Response;
        model.radioMessageType = RadioMessageType::Pair;

        // when
        auto res = obj.parse(model);

        // then
        TEST_ASSERT_EQUAL(RadioMessageResult::OK, res);
        TEST_ASSERT_EQUAL(MessageDirection::Response, model.messageDirection);
        TEST_ASSERT_EQUAL(RadioMessageType::Pair, model.radioMessageType);
    }

    void process()
    {
        RUN_TEST(test_RadioMessage_build_emptyModel);
        RUN_TEST(test_RadioMessage_build_filledBuffer);
        RUN_TEST(test_RadioMessage_build);
        RUN_TEST(test_RadioMessage_parse_emptyBuffer);
        RUN_TEST(test_RadioMessage_parse_messageMalformed);
        RUN_TEST(test_RadioMessage_parse);
    }
};