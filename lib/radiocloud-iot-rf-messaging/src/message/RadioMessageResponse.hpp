#pragma once
#include "RadioMessage.hpp"
#include "models/RadioMessageResponseModel.hpp"

class RadioMessageResponse : public RadioMessage
{
public:
    RadioMessageResponse(MessageBuffer &messageBuffer)
    : RadioMessage(messageBuffer)
    {}

    virtual int parse(RadioMessageModel &radioMessageModel, bool verify = true) override
    {
        auto res = RadioMessage::parse(radioMessageModel, verify);
        if(res != RadioMessageResult::OK){ return res; }

        res = checkResult(reinterpret_cast<RadioMessageResponseModel&>(radioMessageModel));
        if(res != RadioMessageResult::OK){ return res; }

        return RadioMessageResult::OK;
    }

    virtual int build(RadioMessageModel &radioMessageModel) override
    {
        auto res = RadioMessage::build(radioMessageModel);
        if(res != RadioMessageResult::OK){ return res; }

        res = buildResult(reinterpret_cast<RadioMessageResponseModel&>(radioMessageModel));
        if(res != RadioMessageResult::OK){ return res; }

        return RadioMessageResult::OK;
    }

protected:
    int checkResult(RadioMessageResponseModel &radioMessageResponseModel)
    {
        auto sResult = strtok(NULL, messageBuffer_.getDelimeter());
        if(!sResult){ return RadioMessageResult::P_MISSING_RESULT; }

        radioMessageResponseModel.result = static_cast<RadioMessageResponseModel::Result>(atoi(sResult));

        return RadioMessageResult::OK;
    }

    int buildResult(RadioMessageResponseModel &radioMessageResponseModel)
    {
        messageBuffer_.appendDelimeter();
        messageBuffer_.appendLong(static_cast<long>(radioMessageResponseModel.result));

        return RadioMessageResult::OK;
    }

private:
};