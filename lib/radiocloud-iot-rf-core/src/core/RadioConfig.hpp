#pragma once

#include <string.h>
#include <stdint.h>

struct RadioConfig{
    bool isEmpty() { return networkId == 0xff; }

    void setEmpty(){ networkId = 0xff; }

    size_t size(){ return sizeof(RadioConfig); }

    uint8_t networkId;
    uint8_t gatewayId;
    unsigned long customFrequency = 0;
    char encryptKey[17];
    int8_t rssi = -80;
};