#pragma once

#include <unity.h>
#include "utils/MessageBuffer.hpp"

namespace test_MessageBuffer
{
    void test_createObject_noArgs(void) {
        // given
        // when
        MessageBuffer obj;

        // then
        TEST_ASSERT_TRUE(sizeof(obj) > 0);
    }

    void test_length_emptyBuffer(void) {
        // given
        MessageBuffer obj;

        // when
        // then
        TEST_ASSERT_EQUAL(0, obj.length());
    }

    void process()
    {
        RUN_TEST(test_createObject_noArgs);
        RUN_TEST(test_length_emptyBuffer);
    }
};