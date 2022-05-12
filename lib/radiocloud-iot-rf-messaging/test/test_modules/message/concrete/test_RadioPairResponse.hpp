#pragma once

#include <unity.h>
#include "utils/MessageBuffer.hpp"
#include "message/concrete/RadioPairResponse.hpp"
#include "message/models/RadioPairResponseModel.hpp"

namespace test_RadioPairResponse
{
    MessageBuffer buffer_;

    void test_RadioPairResponse_build_ok(void) {
        // given
        RadioPairResponse obj(buffer_);
        RadioPairResponseModel model;
        model.customFrequency = 869000000;
        model.encryptKey = "testKey";
        model.gatewayId = 100;
        model.networkId = 10;
        model.rssi = -66;

        // when
        auto res = obj.build(model);

        // then
        TEST_ASSERT_EQUAL(RadioMessageResult::OK, res);
        TEST_ASSERT_EQUAL_STRING("1|2|0|100|10|869000000|testKey|-66", buffer_.data());
    }

    void test_RadioPairResponse_build_error(void) {
        // given
        RadioPairResponse obj(buffer_);
        RadioPairResponseModel model;
        model.result = RadioMessageResponseModel::Result::ERROR;

        // when
        auto res = obj.build(model);

        // then
        TEST_ASSERT_EQUAL(RadioMessageResult::OK, res);
        TEST_ASSERT_EQUAL_STRING("1|2|1", buffer_.data());
    }

    void test_RadioPairResponse_parse_error(void) {
        // given
        buffer_ = "1|2|1";
        RadioPairResponse obj(buffer_);
        RadioPairResponseModel model;

        // when
        auto res = obj.parse(model);

        // then
        TEST_ASSERT_EQUAL(RadioMessageResult::OK, res);
        TEST_ASSERT_EQUAL(RadioMessageResponseModel::Result::ERROR, model.result);
    }

    void test_RadioPairResponse_parse_ok(void) {
        // given
        buffer_ = "1|2|0|100|10|869000000|testKey|-66";
        RadioPairResponse obj(buffer_);
        RadioPairResponseModel model;

        // when
        auto res = obj.parse(model);

        // then
        TEST_ASSERT_EQUAL(RadioMessageResult::OK, res);
        TEST_ASSERT_EQUAL(RadioMessageResponseModel::Result::OK, model.result);
        TEST_ASSERT_EQUAL(100, model.gatewayId);
        TEST_ASSERT_EQUAL(10, model.networkId);
        TEST_ASSERT_EQUAL(869000000, model.customFrequency);
        TEST_ASSERT_EQUAL_STRING("testKey", model.encryptKey);
        TEST_ASSERT_EQUAL(-66, model.rssi);
    }

    void process()
    {
        RUN_TEST(test_RadioPairResponse_build_ok);
        RUN_TEST(test_RadioPairResponse_build_error);
        RUN_TEST(test_RadioPairResponse_parse_error);
        RUN_TEST(test_RadioPairResponse_parse_ok);
    }
};