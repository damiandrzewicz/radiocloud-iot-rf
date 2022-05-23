#pragma once

class IMessageBase
{
public:

    enum class Direction
    {
        Reqeust = 1,
        Response
    };

    virtual ~IMessageBase(){}
    
    /**
     * @brief Parses message from buffer and converts to model
     *
     * @return int Returns 0 if parse ok or error_code > 0
     */
    virtual bool parse() = 0;

    /**
     * @brief Builds string message from model
     * 
     * @return int Returns 0 if parse ok or error_code > 0
     */
    virtual bool build() = 0;
};

