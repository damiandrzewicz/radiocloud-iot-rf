#pragma once

#include <Arduino.h>

class IMessage
{
public:
    virtual ~IMessage(){}
    
    virtual bool parse(bool verify = true) = 0;
    virtual bool build() = 0;
};

