#pragma once

#include <string.h>
#include <stdint.h>

struct UUIDConfig{

    bool isEmpty() { return uuidNumber[0] == 0xff; }

    void setEmpty(){ uuidNumber[0] = 0xff; }

    size_t size(){ return sizeof(UUIDConfig); }

    uint8_t uuidNumber[16];
};