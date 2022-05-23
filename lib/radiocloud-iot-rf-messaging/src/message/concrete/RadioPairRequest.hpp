#pragma once

#include "../RadioMessageRequest.hpp"
#include "../IMessageModel.hpp"

class RadioPairRequest : public RadioMessageRequest
{
public:

    struct Model
    {

    };

    RadioPairRequest(MessageBuffer &messageBuffer)
    : RadioMessageRequest(messageBuffer)
    {}

    virtual bool parse() override
    {
        // auto res = RadioMessageResponse::parse(radioMessageModel, verify);
        // if(res != RadioMessageResult::OK){ return res; }

        // auto &model = reinterpret_cast<RadioPairRequestModel&>(radioMessageModel);
        // if(model.result == RadioMessageResponseModel::Result::ERROR){ return RadioMessageResult::OK; }

        // auto delim = messageBuffer_.getDelimeter();

        // auto sGatewayId = strtok(NULL, delim);
        // auto sNetworkId = strtok(NULL, delim);
        // auto sCustomFrequency = strtok(NULL, delim);
        // auto sEncryptKey = strtok(NULL, delim);
        // auto sRssi = strtok(NULL, delim);

        // if(!sGatewayId || !sNetworkId || !sCustomFrequency || !sEncryptKey || !sRssi){ return RadioMessageResult::MESSAGE_MALFORMED; }
        
        // model.gatewayId = atoi(sGatewayId);
        // model.networkId = atoi(sNetworkId);
        // model.customFrequency = atol(sCustomFrequency);
        // model.encryptKey = sEncryptKey;
        // model.rssi = atoi(sRssi);

        // return RadioMessageResult::OK;

        return false;
    }

    virtual bool build() override
    {
        // auto res = RadioMessageResponse::build(radioMessageModel);
        // if(res != RadioMessageResult::OK){ return res; }

        // const auto &model = reinterpret_cast<RadioPairRequestModel&>(radioMessageModel);
        // if(model.result == RadioMessageResponseModel::Result::ERROR){ return RadioMessageResult::OK; }

        // messageBuffer_.appendDelimeter();
        // messageBuffer_.appendLong(model.gatewayId);
        // messageBuffer_.appendDelimeter();
        // messageBuffer_.appendLong(model.networkId);
        // messageBuffer_.appendDelimeter();
        // messageBuffer_.appendLong(model.customFrequency);
        // messageBuffer_.appendDelimeter();
        // messageBuffer_.appendText(model.encryptKey);
        // messageBuffer_.appendDelimeter();
        // messageBuffer_.appendLong(model.rssi);

        // return RadioMessageResult::OK;

        return false;
    }

protected:

private:
};