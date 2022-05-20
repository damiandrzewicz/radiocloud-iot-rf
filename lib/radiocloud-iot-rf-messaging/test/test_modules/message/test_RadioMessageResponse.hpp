// #pragma once

// #include <unity.h>
// #include "utils/MessageBuffer.hpp"
// #include "message/RadioMessageResponse.hpp"
// #include "message/models/RadioMessageResponseModel.hpp"

// namespace test_RadioMessageResponse
// {
//     MessageBuffer buffer_;

//     void test_RadioMessageResponse_build_ok(void) {
//         // given
//         RadioMessageResponse obj(buffer_);
//         RadioMessageResponseModel model;
//         model.messageDirection = MessageDirection::Response;
//         model.radioMessageType = RadioMessageType::Pair;

//         // when
//         auto res = obj.build(model);

//         // then
//         TEST_ASSERT_EQUAL(RadioMessageResult::OK, res);
//         TEST_ASSERT_EQUAL_STRING("1|2|0", buffer_.data());
//     }

//     void test_RadioMessageResponse_build_error(void) {
//         // given
//         RadioMessageResponse obj(buffer_);
//         RadioMessageResponseModel model;
//         model.messageDirection = MessageDirection::Response;
//         model.radioMessageType = RadioMessageType::Pair;
//         model.result = RadioMessageResponseModel::Result::ERROR;

//         // when
//         auto res = obj.build(model);

//         // then
//         TEST_ASSERT_EQUAL(RadioMessageResult::OK, res);
//         TEST_ASSERT_EQUAL_STRING("1|2|1", buffer_.data());
//     }

//     void test_RadioMessageResponse_parse_emptyResult(void) {
//         // given
//         buffer_ = "1|2";
//         RadioMessageResponse obj(buffer_);
//         RadioMessageResponseModel model;
//         model.messageDirection = MessageDirection::Response;
//         model.radioMessageType = RadioMessageType::Pair;

//         // when
//         auto res = obj.parse(model);

//         // then
//         TEST_ASSERT_EQUAL(RadioMessageResult::MISSING_RESULT, res);
//     }

//     void test_RadioMessageResponse_parse(void) {
//         // given
//         buffer_ = "1|2|0";
//         RadioMessageResponse obj(buffer_);
//         RadioMessageResponseModel model;
//         model.messageDirection = MessageDirection::Response;
//         model.radioMessageType = RadioMessageType::Pair;

//         // when
//         auto res = obj.parse(model);

//         // then
//         TEST_ASSERT_EQUAL(RadioMessageResult::OK, res);
//         TEST_ASSERT_EQUAL(RadioMessageResponseModel::Result::OK, model.result);
//     }

//     void process()
//     {
//         RUN_TEST(test_RadioMessageResponse_build_ok);
//         RUN_TEST(test_RadioMessageResponse_build_error);
//         RUN_TEST(test_RadioMessageResponse_parse_emptyResult);
//         RUN_TEST(test_RadioMessageResponse_parse);
//     }
// };