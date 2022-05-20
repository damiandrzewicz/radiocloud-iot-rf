// #pragma once

// #include <unity.h>
// #include "utils/MessageBuffer.hpp"
// #include "message/concrete/RadioPairResponse.hpp"

// namespace test_RadioPairResponse
// {
//     MessageBuffer buffer_;

//     void test_RadioPairResponse_build_ok(void) {
//         // given
//         RadioPairResponse obj(buffer_);

//         auto &model = obj.model();
//         model.customFrequency = 869000000;
//         model.encryptKey = "testKey";
//         model.gatewayId = 100;
//         model.networkId = 10;
//         model.rssi = -66;

//         // when
//         auto res = obj.build();

//         // then
//         TEST_ASSERT_TRUE(res.isOk());
//         TEST_ASSERT_EQUAL_STRING("1|2|0|100|10|869000000|testKey|-66", buffer_.data());
//     }

//     void test_RadioPairResponse_build_error(void) {
//         // given
//         RadioPairResponse obj(buffer_);
//         obj.model().result().setError();

//         // when
//         auto res = obj.build();

//         // then
//         TEST_ASSERT_TRUE(res.isOk());
//         TEST_ASSERT_FALSE(res.model().result().isOk());
//         TEST_ASSERT_EQUAL_STRING("1|2|1", buffer_.data());
//     }

//     void test_RadioPairResponse_parse_error(void) {
//         // given
//         buffer_ = "1|2|1";
//         RadioPairResponse obj(buffer_);

//         // when
//         auto res = obj.parse();

//         // then
//         TEST_ASSERT_EQUAL(res.isOk());
//         TEST_ASSERT_TRUE(obj.model().result().isError());
//     }

//     void test_RadioPairResponse_parse_ok(void) {
//         // given
//         buffer_ = "1|2|0|100|10|869000000|testKey|-66";
//         RadioPairResponse obj(buffer_);

//         // when
//         auto res = obj.parse();

//         // then
//         TEST_ASSERT_TRUE(res.isOk());
//         TEST_ASSERT_TRUE(obj.model().result().isOk());
//         TEST_ASSERT_EQUAL(100, obj.model().gatewayId);
//         TEST_ASSERT_EQUAL(10, obj.model().networkId);
//         TEST_ASSERT_EQUAL(869000000, obj.model().customFrequency);
//         TEST_ASSERT_EQUAL_STRING("testKey", obj.model().encryptKey);
//         TEST_ASSERT_EQUAL(-66, obj.model().rssi);
//     }

//     void process()
//     {
//         RUN_TEST(test_RadioPairResponse_build_ok);
//         RUN_TEST(test_RadioPairResponse_build_error);
//         RUN_TEST(test_RadioPairResponse_parse_error);
//         RUN_TEST(test_RadioPairResponse_parse_ok);
//     }
// };