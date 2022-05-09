#pragma once
#include <cstdint>
#include "IMessage.hpp"
#include "utils/MessageBuffer.hpp"
#include "RadioMessageResult.hpp"
#include "RadioMessageType.hpp"
#include "MessageDirection.hpp"
#include "models/RadioMessageModel.hpp"

class RadioMessage : public IMessage
{
public:
    RadioMessage(MessageBuffer &messageBuffer)
    : messageBuffer_(messageBuffer)
    {
    }

    virtual ~RadioMessage(){}

    virtual int parse(RadioMessageModel &radioMessageModel, bool verify = true) override
    {
        if(!messageBuffer_.length())
        {
            return RadioMessageParseResult::VERIFY_ERROR;
        }

        auto delim = messageBuffer_.getDelimeter();

        auto sType = strtok(messageBuffer_.data(), delim);
        auto sDirection = strtok(NULL, delim);
        
        if(!sType){ return RadioMessageParseResult::MESSAGE_TYPE_MISSING; }
        if(!sDirection){ return RadioMessageParseResult::MESSAGE_DIRECTION_MISSING; }

        auto type = static_cast<RadioMessageType>(atoi(sType));
        auto direction = static_cast<MessageDirection>(atoi(sDirection));

        if(verify && (radioMessageModel.radioMessageType != type || radioMessageModel.messageDirection != direction))
        {
            return RadioMessageParseResult::MODEL_MISMATCH;
        }

        radioMessageModel.radioMessageType = type;
        radioMessageModel.messageDirection = direction;

        return RadioMessageParseResult::OK;
    }

    virtual int build(RadioMessageModel &radioMessageModel) override
    {
        messageBuffer_.clear();
        messageBuffer_.appendLong(static_cast<uint8_t>(radioMessageModel.radioMessageType));
        messageBuffer_.appendDelimeter();
        messageBuffer_.appendLong(static_cast<uint8_t>(radioMessageModel.messageDirection));

        return RadioMessageParseResult::OK;
    }

protected:
    bool verifyBufferLength()
    {
        return messageBuffer_.length();
    }

private:
    MessageBuffer &messageBuffer_;
};