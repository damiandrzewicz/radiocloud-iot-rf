#pragma once
#include "RadioMessage.hpp"

class RadioMessageRequest : public RadioMessage
{
public:
    RadioMessageRequest(MessageBuffer &messageBuffer)
    : RadioMessage(messageBuffer)
    {
    }

    RadioMessageRequest(MessageBuffer &messageBuffer, Metadata metadata)
    : RadioMessage(messageBuffer, metadata)
    {
    }

protected:

private:
};