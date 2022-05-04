#pragma once

#include <Arduino.h>

template<class T>
class MessageModel
{
public:
    virtual ~MessageModel(){}

    T &model(){ return model_; }

    virtual bool parseModel() = 0;
    virtual bool buildModel() = 0;

private:
    T model_;
};
