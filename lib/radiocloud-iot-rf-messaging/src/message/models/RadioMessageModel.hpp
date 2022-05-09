#pragma once
#include "../RadioMessageType.hpp"
#include "../MessageDirection.hpp"

struct RadioMessageModel
{
    RadioMessageType radioMessageType;
    MessageDirection   messageDirection;
};