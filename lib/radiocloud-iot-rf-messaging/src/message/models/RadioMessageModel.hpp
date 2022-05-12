#pragma once
#include "../RadioMessageType.hpp"
#include "../MessageDirection.hpp"

struct RadioMessageModel
{
    RadioMessageType radioMessageType = RadioMessageType::Nan;
    MessageDirection messageDirection = MessageDirection::Nan;
};