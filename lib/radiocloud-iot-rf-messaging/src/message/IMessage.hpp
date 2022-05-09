#pragma once

class RadioMessageModel;

class IMessage
{
public:
    virtual ~IMessage(){}
    
    /**
     * @brief Parses message from buffer and converts to model
     * 
     * @param verify 
     * @return int Returns 0 if parse ok or error_code > 0
     */
    virtual int parse(RadioMessageModel &radioMessageModel, bool verify = true) = 0;

    /**
     * @brief Builds string message from model
     * 
     * @param model 
     * @return int Returns 0 if parse ok or error_code > 0
     */
    virtual int build(RadioMessageModel &radioMessageModel) = 0;
};

