#pragma once

struct RadioConnConfig
{
    uint8_t networkId;
    uint8_t gatewayId;
    unsigned long customFrequency = 0;
    const char *encryptKey;
    int8_t rssi = -80;
};