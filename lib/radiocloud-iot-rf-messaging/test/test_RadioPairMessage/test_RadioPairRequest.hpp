#pragma once

#include <unity.h>
#include "utils/MessageBuffer.hpp"
#include "message/concrete/RadioPairRequest.hpp"

namespace test_RadioPairRequest
{
    MessageBuffer buffer_;

    void test_RadioPairRequest_build()
    {
        // given
        RadioPairRequest obj(buffer_);

        // when
        TEST_ASSERT_TRUE(obj.build());

        // then
        TEST_ASSERT_EQUAL(RadioMessage::Type::Pair, obj.metadata().type());
        TEST_ASSERT_EQUAL(RadioMessage::Direction::Reqeust, obj.metadata().direction());
        TEST_ASSERT_EQUAL_STRING("1|1", buffer_.data());
    }

    void test_RadioPairRequest_parse()
    {
        // given
        RadioPairRequest obj(buffer_);

        // when
        buffer_ = "1|1";
        TEST_ASSERT_TRUE(obj.parse());

        // then
        TEST_ASSERT_EQUAL(RadioMessage::Type::Pair, obj.metadata().type());
        TEST_ASSERT_EQUAL(RadioMessage::Direction::Reqeust, obj.metadata().direction());
    }

    void process()
    {
        RUN_TEST(test_RadioPairRequest_build);
        RUN_TEST(test_RadioPairRequest_parse);
    }
};

