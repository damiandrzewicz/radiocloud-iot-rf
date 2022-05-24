#pragma once

#include "../RadioMessageRequest.hpp"
#include "../IMessageModel.hpp"

class RadioPairRequest : public RadioMessageRequest
{
public:

    RadioPairRequest(MessageBuffer &messageBuffer)
    : RadioMessageRequest(messageBuffer, { RadioMessage::Type::Pair, RadioMessage::Direction::Reqeust })
    {}
    
protected:

private:
};