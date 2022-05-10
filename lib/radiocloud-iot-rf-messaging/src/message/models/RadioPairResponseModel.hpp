#pragma once
#include <cstdint>
#include "RadioMessageResponseModel.hpp"

struct RadioPairResponseModel : protected RadioMessageResponseModel
{
    RadioPairResponseModel()
    {
        radioMessageType = RadioMessageType::Pair;
        messageDirection = MessageDirection::Response;
    }

    uint8_t networkId;
    uint8_t gatewayId;
    uint32_t customFrequency;
    const char *encryptKey;
    int8_t rssi;
};