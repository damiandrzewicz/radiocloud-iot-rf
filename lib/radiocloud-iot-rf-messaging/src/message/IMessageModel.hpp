#pragma once

template<typename T>
class IMessageModel
{
public:
    virtual ~IMessageModel(){}

    T &model(){ return model_; }
    const T &model() const { return model_; }

private:
    T model_;
};

