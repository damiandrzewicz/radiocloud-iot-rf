#pragma once

#include <unity.h>
#include "utils/MessageBuffer.hpp"
#include "message/RadioMessageResponse.hpp"

MessageBuffer buffer_;

void test_build_ok()
{
    // given
    auto type = RadioMessage::Type::Pair;
    auto dir = RadioMessage::Direction::Reqeust;
    RadioMessageResponse rm{buffer_, { type, dir }};

    // when
    TEST_ASSERT_TRUE(rm.build());

    // then
    TEST_ASSERT_EQUAL_STRING("1|1|0", buffer_.data());
}

void test_build_error()
{
    auto type = RadioMessage::Type::Pair;
    auto dir = RadioMessage::Direction::Reqeust;
    RadioMessageResponse rm{buffer_, { type, dir }};
    rm.setResponseError();

    // when
    TEST_ASSERT_TRUE(rm.build());

    // then
    TEST_ASSERT_EQUAL_STRING("1|1|1", buffer_.data());
}

void test_parseError_verify()
{
    // given
    auto type = RadioMessage::Type::Pair;
    auto dir = RadioMessage::Direction::Reqeust;
    RadioMessageResponse rm{buffer_, { type, dir }};

    // when
    buffer_ = "1|1|1";
    TEST_ASSERT_TRUE(rm.parse());

    // then
    TEST_ASSERT_TRUE(rm.isResponseError());
}

void test_parseOk_verify()
{
    // given
    auto type = RadioMessage::Type::Pair;
    auto dir = RadioMessage::Direction::Reqeust;
    RadioMessageResponse rm{buffer_, { type, dir }};

    // when
    buffer_ = "1|1|0";
    TEST_ASSERT_TRUE(rm.parse());

    // then
    TEST_ASSERT_FALSE(rm.isResponseError());
}

void test_parseOk_doNotVerify()
{
    // given
    RadioMessageResponse rm{buffer_};

    // when
    buffer_ = "1|1|0";
    TEST_ASSERT_TRUE(rm.parse());

    // then
    TEST_ASSERT_FALSE(rm.isResponseError());
    TEST_ASSERT_EQUAL(RadioMessage::Type::Pair, rm.metadata().type());
    TEST_ASSERT_EQUAL(RadioMessage::Direction::Reqeust, rm.metadata().direction());
}

void process_RadioMessageResponse_tests()
{
    RUN_TEST(test_build_ok);
    RUN_TEST(test_build_error);
    RUN_TEST(test_parseError_verify);
    RUN_TEST(test_parseOk_verify);
    RUN_TEST(test_parseOk_doNotVerify);
}