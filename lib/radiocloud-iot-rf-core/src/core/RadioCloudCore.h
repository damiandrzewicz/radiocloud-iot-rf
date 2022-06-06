
#pragma once

#include <Arduino.h>
#include "RFM69_ATC.h"
#include "../../../radiocloud-iot-rf-messaging/src/utils/MessageBuffer.hpp"
//#include "utils/MessageBuffer.hpp"
#include "UUIDConfig.hpp"


//#include "utils/Portability.hpp"
#include "RadioConfig.hpp"

class RadioCloudCore
{
public:
    struct RadioPinConfig
    {
        uint8_t ssPin;
        uint8_t irqPin;
    };

    RadioCloudCore(RadioPinConfig radioPinConfig = {RF69_SPI_CS, RF69_IRQ_PIN});
    virtual ~RadioCloudCore();

    virtual void setup();
    virtual void loop();

protected:

    // Radio operations
    bool radioSetup(uint8_t nodeId, const RadioConfig &config);
    bool radioSend(uint8_t gatewayId, bool ack);
    bool radioPayloadToBuffer();
    void sendACKRepsonse(const MessageBuffer *request);
    void initRadioConfigForPairing();

    void readRadioConfigFromEEPROM();
    void readUuidFromEEPROM();

protected:
    RFM69_ATC radio_;
    MessageBuffer messageBuffer_;

    UUIDConfig uuidConfig_;
    RadioConfig radioConfig_;
    RadioConfig radioConfigPairing_;
};