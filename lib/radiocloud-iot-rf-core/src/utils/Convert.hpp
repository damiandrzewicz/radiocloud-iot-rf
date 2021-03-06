#pragma once

#include <stdio.h>
#include <avr/pgmspace.h>

class Convert
{
public:
    static void uuid_b2x(const uint8_t *arr, char *buffer, size_t bufferSize)
    {
        snprintf_P(buffer, bufferSize, PSTR("%02X%02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X"), 
            arr[0], arr[1], arr[2], arr[3], arr[4], arr[5], arr[6], arr[7], arr[8], arr[9], arr[10], arr[11], arr[12], arr[13], arr[14], arr[15]);
    }
};