#pragma once

#include "../RadioMessageRequest.hpp"
#include "../IMessageModel.hpp"

struct RadioRegisterRequestModel
{
    const char *uuid;
};

class RadioRegisterRequest : public RadioMessageRequest, public IMessageModel<RadioRegisterRequestModel>
{
public:

    RadioRegisterRequest(MessageBuffer &messageBuffer)
    : RadioMessageRequest(messageBuffer, { RadioMessage::Type::Register, RadioMessage::Direction::Reqeust })
    {}

    virtual bool parse() override
    {
        if(!RadioMessageRequest::parse()){ return false; }

        auto delim = messageBuffer_.getDelimeter();

        auto sUuid = strtok(NULL, delim);

        if(!sUuid)
        { 
            lastProcessError_ = ProcessError::ArgumentsMismatch;
            return false;
        }

        auto &m = model();
        
        m.uuid = sUuid;

        return true;
    }

    virtual bool build() override
    {
        if(!RadioMessageRequest::build()){ return false; }

        auto &m = model();

        messageBuffer_.appendDelimeter();
        messageBuffer_.appendText(m.uuid);

        return true;
    }
    
protected:

private:
};