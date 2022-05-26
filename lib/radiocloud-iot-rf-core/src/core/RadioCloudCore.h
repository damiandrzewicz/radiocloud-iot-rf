
#pragma once

#include <Arduino.h>
#include <ArduinoLog.h>
#include <EEPROM.h>
#include "RFM69_ATC.h"
#include "utils/MessageBuffer.hpp"
#include "YA_FSM.h"
#include "UUIDConfig.hpp"
#include "utils/Convert.hpp"

class RadioCloudCore
{
public:
    struct RadioPinConfig
    {
        uint8_t ssPin;
        uint8_t irqPin;
    };

    struct RadioConnConfig
    {
        uint8_t networkId;
        uint8_t gatewayId;
        unsigned long customFrequency = 0;
        const char *encryptKey;
        int8_t rssi = -80;
    };

    RadioCloudCore(RadioPinConfig radioPinConfig = {RF69_SPI_CS, RF69_IRQ_PIN})
    {
        radio_ = { radioPinConfig.ssPin, radioPinConfig.irqPin };
    }

    virtual ~RadioCloudCore(){}

    virtual void setup()
    {
        readUuid();
        initStateMachine(); 
    }

    virtual void loop()
    {
        // StateMachine loop
        stateMachine_.Update();
    }

protected:
    virtual void initStateMachine() = 0;

    // Radio operations
    bool radioSetup(uint8_t nodeId, const RadioConnConfig &config)
    {
        Log.noticeln(F("RadioConfig(nodeId=%d, networkId=%d, rssi=%d, customFreq=%u, encryptKey=%s, gatewayId=%d"), 
            nodeId, config.networkId, config.rssi, config.customFrequency, config.encryptKey, config.gatewayId);

        if(radio_.initialize(RF69_868MHZ, nodeId, config.networkId))
        {
            if(config.customFrequency)
            {
                radio_.setFrequency(config.customFrequency);
            }

            radio_.setHighPower();
            radio_.enableAutoPower(config.rssi);
            radio_.encrypt(config.encryptKey);
            radio_.sleep();

            Log.noticeln(F("setup ok!"));

            return true;
        }
        else
        {
            Log.fatalln(F("setup failed!"));
            return false;
        }
    }

    bool radioSend(uint8_t gatewayId, bool ack)
    {
        //Log.verboseln(F("Appliance::radioSend"));

        auto data = messageBuffer_.data();
        auto size = strlen(data);

        Log.noticeln(F("[RADIO OUT<<<<]: data=%s, size=%d, ack=%d, gatewayId=%d"), data, size, ack, gatewayId);

        if(ack)
        {
            if(radio_.sendWithRetry(gatewayId, data, size, 2, 100U))
            {
                Log.noticeln(F("send ok!"));
                if(radioPayloadToBuffer())
                {
                    Log.noticeln(F("got payload into buffer!"));
                    return true;
                }
                else
                {
                    Log.warningln(F("missing response!"));
                    return false;
                }
            }
            else
            {
                Log.warningln(F("radio sent FAILED!"));
                return false;
            }
        }
        else
        {
            radio_.send(gatewayId, data, size);
            return true;
        }
    }

    bool radioPayloadToBuffer()
    {
        //Log.verboseln(F("Appliance::radioPayloadToBuffer"));

        if (radio_.DATALEN) 
        {
            auto data = reinterpret_cast<const char*>(radio_.DATA);
            auto size = strlen(data);
            
            if(radio_.DATALEN == size)  // got a valid packet?
            {
                messageBuffer_ = data;  // copy message to the buffer
                Log.noticeln(F("[RADIO IN>>>>]: data=%s, size=%d, senderId=%d, rssi=%d"), messageBuffer_.data(), size, radio_.SENDERID, radio_.RSSI);
                return true;
            }
        }

        return false;
    }

    void sendACKRepsonse(const MessageBuffer *request)
    {
        //Log.verboseln(F("Appliance::sendACKRepsonse"));
        // Do not add logs before send ACK because of timing issues!

        if(request)
        {
            auto data = request->data();
            auto size = strlen(data);
            radio_.sendACK(data, size);
            Log.noticeln(F("[RADIO OUT(AKC)<<<<]: data=%s, size=%d, senderId=%d"), data, size, radio_.SENDERID);
        }
        else
        {
            radio_.sendACK();
            Log.noticeln(F("[RADIO OUT(AKC)<<<<]: EMPTY, senderId=%d"),  radio_.SENDERID);
        }
    }

    void readUuid()
    {
        EEPROM.get(0, uuidConfig_); 

        if(uuidConfig_.isEmpty())
        {
            TrueRandom.uuid(uuidConfig_.uuidNumber);
            EEPROM.put(100, uuidConfig_);
        }

        char uuidHex[40];
        Convert::uuid_b2x(uuidConfig_.uuidNumber, uuidHex, sizeof(uuidHex));
        Log.noticeln(F("uuid: %s"), uuidHex);
    }

private:
    RFM69_ATC radio_;
    MessageBuffer messageBuffer_;
    YA_FSM stateMachine_;
    UUIDConfig uuidConfig_;
};