#pragma once

#include <Arduino.h>

class Delay
{
public:
    static void delayBlock(unsigned long ms)
    {
        delay(ms);
    }

    struct DelayNoBlockInfo
    {
        unsigned long start_time; 
        unsigned long current_time;
    };

    static bool delayNoBlock(DelayNoBlockInfo &info, unsigned long ms)
    {
        info.current_time = millis(); // update the timer every cycle
        bool timeout = false;

        if (info.current_time - info.start_time >= ms) {
            info.start_time = info.current_time;  // reset the timer
            timeout = true;
        }

        return timeout;
    }
};