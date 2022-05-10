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
    }

    virtual int build(RadioMessageModel &radioMessageModel) override
    {
        

        return RadioMessageResult::ERROR;
    }

protected:
    int checkResult(RadioMessageResponseModel &radioMessageResponseModel)
    {
        auto delim = messageBuffer_.getDelimeter();

        auto sResult = strtok(NULL, delim);
        if(!sResult){ return RadioMessageResult::P_MISSING_RESULT; }

        radioMessageResponseModel.result = static_cast<RadioMessageResponseModel::Result>(atoi(sResult));

        return RadioMessageResult::OK;
    }


private:
};