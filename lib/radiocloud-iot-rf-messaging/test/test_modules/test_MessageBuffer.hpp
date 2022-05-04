#pragma once

#include <unity.h>
#include "utils/MessageBuffer.hpp"

namespace test_MessageBuffer
{
    /**
     * @brief Empty buffer tests
     * 
     */
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

    void test_data_emptyBuffer(void) {
        // given
        MessageBuffer obj;

        // when
        // then
        TEST_ASSERT_EQUAL_STRING("", obj.data());
    }

    void test_isFilled_emptyBuffer(void) {
        // given
        MessageBuffer obj;

        // when
        // then
        TEST_ASSERT_FALSE(obj.isFilled());
    }

    /**
     * @brief Fill some data tests
     * 
     */

    void test_appendText(void) {
        // given
        MessageBuffer obj;

        // when
        obj.appendText("test");

        // then
        TEST_ASSERT_EQUAL_STRING("test", obj.data());
    }

    void test_length_filledBuffer(void) {
        // given
        MessageBuffer obj;

        // when
        obj.appendText("test");

        // then
        TEST_ASSERT_EQUAL(strlen("test"), obj.length());
    }

    void test_data_filledBuffer(void) {
        // given
        MessageBuffer obj;

        // when
        obj.appendText("test");

        // then
        TEST_ASSERT_EQUAL_STRING("test", obj.data());
    }

    void test_isFilled_filledBuffer(void) {
        // given
        MessageBuffer obj;

        // when
        obj.appendText("test");

        // then
        TEST_ASSERT_TRUE(obj.isFilled());
    }

    void process()
    {
        RUN_TEST(test_createObject_noArgs);
        RUN_TEST(test_length_emptyBuffer);
        RUN_TEST(test_data_emptyBuffer);
        RUN_TEST(test_isFilled_emptyBuffer);
        RUN_TEST(test_appendText);
        RUN_TEST(test_length_filledBuffer);
        RUN_TEST(test_data_filledBuffer);
        RUN_TEST(test_isFilled_filledBuffer);
    }
};