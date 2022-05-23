#pragma once
#include "RadioMessage.hpp"

class RadioMessageResponse : public RadioMessage
{
public:
    RadioMessageResponse(MessageBuffer &messageBuffer)
    : RadioMessage(messageBuffer)
    {
    }

    RadioMessageResponse(MessageBuffer &messageBuffer, Metadata metadata)
    : RadioMessage(messageBuffer, metadata)
    {
    }

    void setResponseError(){ resultError_ = true; }
    bool isResponseError(){ return resultError_; }
    
    virtual bool parse() override
    {
        if(!RadioMessage::parse()){ return false; }

        if(!checkResult()){ return false; }

        return true;
    }

    virtual bool build() override
    {
        if(!RadioMessage::build()){ return false; }

        if(!buildResult()){ return false; }

        return true;
    }

protected:
    bool checkResult()
    {
        auto sResult = strtok(NULL, messageBuffer_.getDelimeter());
        if(!sResult){ 
            lastProcessError_ = RadioMessage::ProcessError::MissingResult;
            return false;
        }

        resultError_ = static_cast<bool>(atoi(sResult));

        return true;
    }

    int buildResult()
    {
        messageBuffer_.appendDelimeter();
        messageBuffer_.appendLong(static_cast<long>(resultError_));

        return true;
    }

private:
    bool resultError_ = false;
};