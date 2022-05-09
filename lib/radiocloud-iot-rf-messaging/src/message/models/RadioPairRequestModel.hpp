#pragma once
#include "RadioMessageModel.hpp"

struct RadioPairRequestModel : protected RadioMessageModel
{
    RadioPairRequestModel()
    {
        radioMessageType = RadioMessageType::Pair;
        messageDirection = MessageDirection::Request;
    }
};