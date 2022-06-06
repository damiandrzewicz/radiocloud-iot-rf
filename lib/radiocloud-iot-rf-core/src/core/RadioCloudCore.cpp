
#include "RadioCloudCore.h"
#include "TrueRandom.h"
#include "utils/Convert.hpp"
#include <EEPROM.h>
#include <ArduinoLog.h>

RadioCloudCore::RadioCloudCore(RadioCloudCore::RadioPinConfig radioPinConfig)
{
    radio_ = { radioPinConfig.ssPin, radioPinConfig.irqPin };
}

RadioCloudCore::~RadioCloudCore()
{

}

void RadioCloudCore::setup()
{
    readUuidFromEEPROM();
    readRadioConfigFromEEPROM();
    initRadioConfigForPairing();
}

void RadioCloudCore::loop()
{
    // StateMachine loop
    //Log.noticeln(F("bUpd"));
    //stateMachine_.Update();
    //Log.noticeln(F("aUpd"));
}


// Radio operations
bool RadioCloudCore::radioSetup(uint8_t nodeId, const RadioConfig &config)
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

bool RadioCloudCore::radioSend(uint8_t gatewayId, bool ack)
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

bool RadioCloudCore::radioPayloadToBuffer()
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

void RadioCloudCore::sendACKRepsonse(const MessageBuffer *request)
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

void RadioCloudCore::initRadioConfigForPairing()
{
    //Log.verboseln(F("Appliance::getRadioConfigForPairing"));

    radioConfigPairing_.gatewayId = 1;
    radioConfigPairing_.networkId = 111;
    radioConfigPairing_.customFrequency = 869000000L;
    strcpy_P(radioConfigPairing_.encryptKey, PSTR("sampleEncryptKey"));
    radioConfigPairing_.rssi = -80;
}

void RadioCloudCore::readRadioConfigFromEEPROM()
{
    EEPROM.get(uuidConfig_.size() + 1, radioConfig_);
}

void RadioCloudCore::readUuidFromEEPROM()
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