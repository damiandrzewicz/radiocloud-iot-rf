#pragma once
#include <cstdint>
#include "IMessage.hpp"
#include "utils/MessageBuffer.hpp"

class RadioMessage : public IMessage
{
public:

    enum class Type
    {
        Pair = 1,
        Register
    };

    class ActionResult
    {
    public:
        enum class Value
        {
            Ok,
            Error,
            MetadataMismatch
        };

        bool isError()const{ return value_ != Value::Ok; }
        bool isOk()const{ return value_ == Value::Ok; }
        Value value()const{ return value_; }

        bool operator==(Value value){ return value == value_; }

    private:
        Value value_;
    };

    class Metadata
    {
    public:
        Metadata() = delete;   // Prevent construction without arguments
        Metadata(Type type, Direction direction){ type_ = type; direction_ = direction; }

        Type type() const { return type_; }
        Direction direction() const {return direction_; }
    
    private:
        Type type_;
        Direction direction_;
    };

    RadioMessage(MessageBuffer &messageBuffer, Metadata metadata)
    : messageBuffer_(messageBuffer), metadata_(metadata)
    {
    }

    virtual ~RadioMessage(){}

    virtual int parse(bool verify = true) override
    {
        // if(!messageBuffer_.length())
        // {
        //     return RadioMessageResult::EMPTY_BUFFER;
        // }

        // auto delim = messageBuffer_.getDelimeter();

        // auto sType = strtok(messageBuffer_.data(), delim);
        // auto sDirection = strtok(NULL, delim);
        
        // if(!sType || !sDirection){ return RadioMessageResult::MESSAGE_MALFORMED; }

        // auto type = static_cast<RadioMessageType>(atoi(sType));
        // auto direction = static_cast<MessageDirection>(atoi(sDirection));

        // if(verify && (radioMessageModel.radioMessageType != type || radioMessageModel.messageDirection != direction))
        // {
        //     return RadioMessageResult::MODEL_MALFORMED;
        // }

        // radioMessageModel.radioMessageType = type;
        // radioMessageModel.messageDirection = direction;

        // return RadioMessageResult::OK;

        return 0;
    }

    virtual int build() override
    {
        // if(radioMessageModel.messageDirection == MessageDirection::Nan 
        //     || radioMessageModel.radioMessageType == RadioMessageType::Nan)
        // {
        //     return RadioMessageResult::MODEL_MALFORMED;
        // }

        // messageBuffer_.clear();
        // messageBuffer_.appendLong(static_cast<uint8_t>(radioMessageModel.radioMessageType));
        // messageBuffer_.appendDelimeter();
        // messageBuffer_.appendLong(static_cast<uint8_t>(radioMessageModel.messageDirection));

        // return RadioMessageResult::OK;

        return 0;
    }

    const Metadata &metadata() const { return metadata_; }

protected:
    MessageBuffer &messageBuffer_;
    

private:    
    Metadata metadata_;

};