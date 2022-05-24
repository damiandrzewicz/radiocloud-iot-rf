#pragma once
#include "../RadioMessageResponse.hpp"
#include "../IMessageModel.hpp"

struct RadioPairResponseModel
{
    uint8_t networkId;
    uint8_t gatewayId;
    uint32_t customFrequency;
    const char *encryptKey;
    int8_t rssi;
};

class RadioPairResponse : public RadioMessageResponse, public IMessageModel<RadioPairResponseModel>
{
public:
    RadioPairResponse(MessageBuffer &messageBuffer)
    : RadioMessageResponse(messageBuffer, { RadioMessage::Type::Pair, RadioMessage::Direction::Response })
    {}

    virtual bool parse() override
    {
        if(!RadioMessageResponse::parse()){ return false; }

        if(isResponseError()){ return true; }

        auto delim = messageBuffer_.getDelimeter();

        auto sGatewayId = strtok(NULL, delim);
        auto sNetworkId = strtok(NULL, delim);
        auto sCustomFrequency = strtok(NULL, delim);
        auto sEncryptKey = strtok(NULL, delim);
        auto sRssi = strtok(NULL, delim);

        if(!sGatewayId || !sNetworkId || !sCustomFrequency || !sEncryptKey || !sRssi)
        { 
            lastProcessError_ = ProcessError::ArgumentsMismatch;
            return false;
        }

        auto &m = model();
        
        m.gatewayId = atoi(sGatewayId);
        m.networkId = atoi(sNetworkId);
        m.customFrequency = atol(sCustomFrequency);
        m.encryptKey = sEncryptKey;
        m.rssi = atoi(sRssi);

        return true;
    }

    virtual bool build() override
    {
        if(!RadioMessageResponse::build()){ return false; }

        if(isResponseError()){ return true; }

        auto &m = model();

        messageBuffer_.appendDelimeter();
        messageBuffer_.appendLong(m.gatewayId);
        messageBuffer_.appendDelimeter();
        messageBuffer_.appendLong(m.networkId);
        messageBuffer_.appendDelimeter();
        messageBuffer_.appendLong(m.customFrequency);
        messageBuffer_.appendDelimeter();
        messageBuffer_.appendText(m.encryptKey);
        messageBuffer_.appendDelimeter();
        messageBuffer_.appendLong(m.rssi);

        return true;
    }

protected:

private:
};