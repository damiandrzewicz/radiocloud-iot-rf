#pragma once

#include "../RadioMessageResponse.hpp"
#include "../IMessageModel.hpp"

struct RadioRegisterResponseModel
{
    int8_t nodeId;
};

class RadioRegisterResponse : public RadioMessageResponse, public IMessageModel<RadioRegisterResponseModel>
{
public:

    RadioRegisterResponse(MessageBuffer &messageBuffer)
    : RadioMessageResponse(messageBuffer, { RadioMessage::Type::Register, RadioMessage::Direction::Response })
    {}

    virtual bool parse() override
    {
        if(!RadioMessageResponse::parse()){ return false; }

        if(isResponseError()){ return true; }

        auto delim = messageBuffer_.getDelimeter();

        auto sNodeId = strtok(NULL, delim);

        if(!sNodeId)
        { 
            lastProcessError_ = ProcessError::ArgumentsMismatch;
            return false;
        }

        auto &m = model();
        m.nodeId = atoi(sNodeId);

        return true;
    }

    virtual bool build() override
    {
        if(!RadioMessageResponse::build()){ return false; }

        if(isResponseError()){ return true; }

        auto &m = model();

        messageBuffer_.appendDelimeter();
        messageBuffer_.appendLong(m.nodeId);

        return true;
    }
    
protected:

private:
};