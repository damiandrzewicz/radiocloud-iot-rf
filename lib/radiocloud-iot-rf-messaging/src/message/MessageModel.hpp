#pragma once

template<class T>
class MessageModel
{
public:
    virtual ~MessageModel(){}

    T &model(){ return model_; }

    virtual bool parse() = 0;
    virtual bool build() = 0;

private:
    T model_;
};
