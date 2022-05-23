#pragma once

#include <unity.h>
#include "utils/MessageBuffer.hpp"
#include "message/concrete/RadioPairRequest.hpp"

namespace test_RadioPairRequest
{
    MessageBuffer buffer_;

    // void test_RadioPairRequest_build(void) {
    //     // given
    //     RadioPairRequest obj(buffer_);

    //     // when
    //     auto res = obj.build();

    //     // then
    //     TEST_ASSERT_TRUE(res.isOk());
    //     TEST_ASSERT_EQUAL_STRING("1|1", buffer_.data());
    // }

    // void test_RadioPairRequest_parse(void) {
    //     // given
    //     buffer_ = "1|1";
    //     RadioPairRequest obj(buffer_);

    //     // when
    //     auto res = obj.parse();

    //     // then
    //     TEST_ASSERT_TRUE(res.isOk());
    //     TEST_ASSERT_EQUAL(RadioMessageType::Pair, obj.getRadioMessageType());
    //     TEST_ASSERT_EQUAL(MessageDirection::Request, obj.getMessageDirection() );
    // }

    void process()
    {
        //RUN_TEST(test_RadioPairRequest_build);
        //RUN_TEST(test_RadioPairRequest_parse);
    }
};

