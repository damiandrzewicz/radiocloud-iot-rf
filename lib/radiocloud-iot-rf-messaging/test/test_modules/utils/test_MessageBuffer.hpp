#pragma once

#include <unity.h>
#include "utils/MessageBuffer.hpp"


/**
 * @brief Empty buffer tests
 * 
 */
void test_MessageBuffer_createObject_noArgs(void) {
    // given
    // when
    MessageBuffer obj;

    // then
    TEST_ASSERT_TRUE(sizeof(obj) > 0);
    TEST_ASSERT_EQUAL_STRING("|", obj.getDelimeter());
}

void test_MessageBuffer_length_emptyBuffer(void) {
    // given
    MessageBuffer obj;

    // when
    // then
    TEST_ASSERT_EQUAL(0, obj.length());
}

void test_MessageBuffer_data_emptyBuffer(void) {
    // given
    MessageBuffer obj;

    // when
    // then
    TEST_ASSERT_EQUAL_STRING("", obj.data());
}

void test_MessageBuffer_isFilled_emptyBuffer(void) {
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

void test_MessageBuffer_appendText(void) {
    // given
    MessageBuffer obj;

    // when
    obj.appendText("test");

    // then
    TEST_ASSERT_EQUAL_STRING("test", obj.data());
}

void test_MessageBuffer_length_filledBuffer(void) {
    // given
    MessageBuffer obj;

    // when
    obj.appendText("test");

    // then
    TEST_ASSERT_EQUAL(strlen("test"), obj.length());
}

void test_MessageBuffer_data_filledBuffer(void) {
    // given
    MessageBuffer obj;

    // when
    obj.appendText("test");

    // then
    TEST_ASSERT_EQUAL_STRING("test", obj.data());
}

void test_MessageBuffer_isFilled_filledBuffer(void) {
    // given
    MessageBuffer obj;

    // when
    obj.appendText("test");

    // then
    TEST_ASSERT_TRUE(obj.isFilled());
}

void test_MessageBuffer_appendLong(void) {
    // given
    MessageBuffer obj;

    // when
    obj.appendLong(4433441);

    // then
    TEST_ASSERT_EQUAL_STRING("4433441", obj.data());
}

void test_MessageBuffer_appendFloat(void) {
    // given
    MessageBuffer obj;

    // when
    obj.appendFloat(12345.7755);

    // then
    TEST_ASSERT_EQUAL_STRING("12345.78", obj.data());
}

void test_MessageBuffer_customDelimeter(void) {
    // given
    MessageBuffer obj("x");

    // when
    // then
    TEST_ASSERT_EQUAL_STRING("x", obj.getDelimeter());
}

void test_MessageBuffer_assignOperator(void) {
    // given
    MessageBuffer obj;
    TEST_ASSERT_EQUAL_STRING("", obj.data());

    // when
    obj = "someData123";
    // then
    TEST_ASSERT_EQUAL_STRING("someData123", obj.data());
}

void test_MessageBuffer_clear(void) {
    // given
    MessageBuffer obj;
    obj.appendText("initialData");
    TEST_ASSERT_EQUAL_STRING("initialData", obj.data());

    // when
    obj.clear();
    
    // then
    TEST_ASSERT_EQUAL_STRING("", obj.data());
}

void test_MessageBuffer_buildMessage(void) {
    // given
    MessageBuffer obj;

    // when
    obj.appendText("text");
    obj.appendDelimeter();
    obj.appendLong(123);
    obj.appendDelimeter();
    obj.appendFloat(1.23);
    
    // then
    TEST_ASSERT_EQUAL_STRING("text|123|1.23", obj.data());
}

namespace test_MessageBuffer
{
    void process()
    {
        RUN_TEST(test_MessageBuffer_createObject_noArgs);
        RUN_TEST(test_MessageBuffer_length_emptyBuffer);
        RUN_TEST(test_MessageBuffer_data_emptyBuffer);
        RUN_TEST(test_MessageBuffer_isFilled_emptyBuffer);
        RUN_TEST(test_MessageBuffer_appendText);
        RUN_TEST(test_MessageBuffer_length_filledBuffer);
        RUN_TEST(test_MessageBuffer_data_filledBuffer);
        RUN_TEST(test_MessageBuffer_isFilled_filledBuffer);
        RUN_TEST(test_MessageBuffer_appendLong);
        RUN_TEST(test_MessageBuffer_appendFloat);
        RUN_TEST(test_MessageBuffer_customDelimeter);
        RUN_TEST(test_MessageBuffer_assignOperator);
        RUN_TEST(test_MessageBuffer_clear);
        RUN_TEST(test_MessageBuffer_buildMessage);
    }
};