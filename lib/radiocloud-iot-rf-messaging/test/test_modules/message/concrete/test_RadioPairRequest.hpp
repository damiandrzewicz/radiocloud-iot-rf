#pragma once

#include <unity.h>
#include "utils/MessageBuffer.hpp"
#include "message/RadioMessageRequest.hpp"
#include "message/models/RadioPairRequestModel.hpp"

namespace test_RadioPairRequest
{
    MessageBuffer buffer_;

    void test_RadioPairRequest_build(void) {
        // given
        RadioMessageRequest obj(buffer_);
        RadioPairRequestModel model;

        // when
        auto res = obj.build(model);

        // then
        TEST_ASSERT_EQUAL(RadioMessageResult::OK, res);
        TEST_ASSERT_EQUAL_STRING("1|1", buffer_.data());
    }

    void test_RadioPairRequest_parse(void) {
        // given
        buffer_ = "1|1";
        RadioMessageRequest obj(buffer_);
        RadioPairRequestModel model;

        // when
        auto res = obj.parse(model);

        // then
        TEST_ASSERT_EQUAL(RadioMessageResult::OK, res);
        TEST_ASSERT_EQUAL(RadioMessageType::Pair, model.radioMessageType);
        TEST_ASSERT_EQUAL(MessageDirection::Request, model.messageDirection );
    }

    void process()
    {
        RUN_TEST(test_RadioPairRequest_build);
        RUN_TEST(test_RadioPairRequest_parse);
    }
};