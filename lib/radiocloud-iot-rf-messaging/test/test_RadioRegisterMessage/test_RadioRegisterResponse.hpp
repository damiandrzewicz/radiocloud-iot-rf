#pragma once

#include <unity.h>
#include "utils/MessageBuffer.hpp"
#include "message/concrete/RadioRegisterResponse.hpp"

namespace test_RadioRegisterResponse
{
    MessageBuffer buffer_;

    void test_RadioRegisterResponse_build_ok()
    {
        // given
        RadioRegisterResponse obj(buffer_);

        // when
        auto &model = obj.model();
        model.nodeId = 55;

        TEST_ASSERT_TRUE(obj.build());

        //then
        TEST_ASSERT_EQUAL_STRING("2|2|0|55", buffer_.data());
    }

    void test_RadioRegisterResponse_build_error(void) {
        // given
        RadioRegisterResponse obj(buffer_);
        obj.setResponseError();

        // when
        auto &model = obj.model();
        model.nodeId = 55;

        TEST_ASSERT_TRUE(obj.build());

        // then

        TEST_ASSERT_EQUAL_STRING("2|2|1", buffer_.data());
    }

    void test_RadioRegisterResponse_parse_error(void) {
        // given
        buffer_ = "2|2|1";
        RadioRegisterResponse obj(buffer_);

        // when
        TEST_ASSERT_TRUE(obj.parse());

        // then
        TEST_ASSERT_TRUE(obj.isResponseError());
    }

    void test_RadioRegisterResponse_parse_ok(void) {
        // given
        buffer_ = "2|2|0|88";
        RadioRegisterResponse obj(buffer_);

        // when
        TEST_ASSERT_TRUE(obj.parse());

        // then
        TEST_ASSERT_EQUAL(88, obj.model().nodeId);
    }

    void process()
    {
        RUN_TEST(test_RadioRegisterResponse_build_ok);
        RUN_TEST(test_RadioRegisterResponse_build_error);
        RUN_TEST(test_RadioRegisterResponse_parse_error);
        RUN_TEST(test_RadioRegisterResponse_parse_ok);
    }
};

