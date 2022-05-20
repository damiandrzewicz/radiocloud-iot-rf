#pragma once

class RadioMessageModel;

class IMessage
{
public:

    enum class Direction
    {
        Reqeust = 1,
        Response
    };

    virtual ~IMessage(){}
    
    /**
     * @brief Parses message from buffer and converts to model
     * 
     * @param verify 
     * @return int Returns 0 if parse ok or error_code > 0
     */
    virtual int parse(bool verify = true) = 0;

    /**
     * @brief Builds string message from model
     * 
     * @return int Returns 0 if parse ok or error_code > 0
     */
    virtual int build() = 0;
};

