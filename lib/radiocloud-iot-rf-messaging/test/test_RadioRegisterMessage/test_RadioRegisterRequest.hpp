#pragma once

#include <unity.h>
#include "utils/MessageBuffer.hpp"
#include "message/concrete/RadioRegisterRequest.hpp"

namespace test_RadioRegisterRequest
{
    MessageBuffer buffer_;

    void test_RadioRegisterRequest_build()
    {
        // given
        RadioRegisterRequest obj(buffer_);
        auto &model = obj.model();
        model.uuid = "123e4567-e89b-12d3-a456-426614174000";
        
        // when
        TEST_ASSERT_TRUE(obj.build());

        // then
        TEST_ASSERT_EQUAL(RadioMessage::Type::Register, obj.metadata().type());
        TEST_ASSERT_EQUAL(RadioMessage::Direction::Reqeust, obj.metadata().direction());
        TEST_ASSERT_EQUAL_STRING("2|1|123e4567-e89b-12d3-a456-426614174000", buffer_.data());
    }

    void test_RadioRegisterRequest_parse()
    {
        // given
        RadioRegisterRequest obj(buffer_);

        // when
        buffer_ = "2|1|123e4567-e89b-12d3-a456-426614174000";
        TEST_ASSERT_TRUE(obj.parse());

        // then
        TEST_ASSERT_EQUAL(RadioMessage::Type::Register, obj.metadata().type());
        TEST_ASSERT_EQUAL(RadioMessage::Direction::Reqeust, obj.metadata().direction());
        TEST_ASSERT_EQUAL_STRING(obj.model().uuid, "123e4567-e89b-12d3-a456-426614174000");
    }

    void process()
    {
        RUN_TEST(test_RadioRegisterRequest_build);
        RUN_TEST(test_RadioRegisterRequest_parse);
    }
};

