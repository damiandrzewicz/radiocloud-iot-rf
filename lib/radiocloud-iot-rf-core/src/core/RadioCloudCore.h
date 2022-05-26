
#pragma once

#include <Arduino.h>
#include <ArduinoLog.h>
#include "RFM69_ATC.h"
#include "RadioConnConfig.h"

class RadioCloudCore
{
public:

    virtual ~RadioCloudCore(){}

    virtual void setup();
    virtual void loop();

protected:
    virtual void initStateMachine() = 0;

    // Radio operations
    bool radioSetup(uint8_t nodeId)
    {
        RadioConnConfig conn = {
            1,
            100,
            0,
            -80,
            ""
        };
    }

private:
};