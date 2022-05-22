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

    enum class ProcessError
    {
        Generic,
        MetadataMismatch,
        EmptyBuffer
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

    virtual bool parse(bool verify = true) override
    {
        if(!messageBuffer_.length())
        {
            lastProcessError_ = ProcessError::EmptyBuffer;
            return false;
        }

        auto delim = messageBuffer_.getDelimeter();

        auto sType = strtok(messageBuffer_.data(), delim);
        auto sDirection = strtok(NULL, delim);
        
        if(!sType || !sDirection)
        { 
            lastProcessError_ = ProcessError::MetadataMismatch;
            return false;
        }

        auto type = static_cast<Type>(atoi(sType));
        auto direction = static_cast<Direction>(atoi(sDirection));

        if(verify && (metadata_.type() != type || metadata_.direction() != direction))
        {
            lastProcessError_ = ProcessError::MetadataMismatch;
            return false;
        }

        metadata_ = Metadata(type, direction);

        return true;
    }

    virtual bool build() override
    {
        messageBuffer_.clear();
        messageBuffer_.appendLong(static_cast<uint8_t>(metadata_.type()));
        messageBuffer_.appendDelimeter();
        messageBuffer_.appendLong(static_cast<uint8_t>(metadata_.direction()));

        return true;
    }

    const Metadata &metadata() const { return metadata_; }
    ProcessError lastProcessError() const { return lastProcessError_; }

protected:
    MessageBuffer &messageBuffer_;
    ProcessError lastProcessError_ = ProcessError::Generic;

private:    
    Metadata metadata_;

};