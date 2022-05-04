#pragma once

#include <cstring>

class MessageBuffer
{
public:
    static const uint8_t MaxSize = 100;

    void appendText(const char *data)
    {
        strcat(buffer_, data);
    }

    char *data()
    {
        return buffer_;
    }

    const char *data() const
    {
        return buffer_;
    }

    uint8_t length()
    {
        return std::strlen(buffer_);
    }

    bool isFilled()
    {
        return length();
    }

private:
    
    char buffer_[MaxSize];
};