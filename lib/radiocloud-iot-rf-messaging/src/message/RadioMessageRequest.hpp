#pragma once
#include "RadioMessage.hpp"

class RadioMessageRequest : public RadioMessage
{
public:
    RadioMessageRequest(MessageBuffer &messageBuffer)
    : RadioMessage(messageBuffer)
    {}

protected:

private:
};