#pragma once

#include <cstring>
#include <cstdio>
#include <stdlib.h>

class MessageBuffer
{
public:
    static const uint8_t MaxSize = 100;

    MessageBuffer(const char *delimeter = "|")
    : delimeter_(delimeter)
    {
        clear();
    }

    void appendText(const char *data)
    {
        strcat(buffer_, data);
    }

    void appendLong(long value)
    {
        snprintf(buffer_ + strlen(buffer_), sizeof(buffer_) - strlen(buffer_), "%ld", value);
    }

    void appendFloat(double value, uint8_t size = 4, uint8_t prec = 2)
    {
        char temp[20];
        dtostrf(value, size, prec, temp);
        strcat(buffer_, temp);
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

    void clear()
    {
        memset(buffer_, 0, sizeof(buffer_));
    }

    void appendDelimeter()
    {
        strcat(buffer_, delimeter_);
    }

    const char *getDelimeter() const
    {
        return delimeter_;
    }

    MessageBuffer &operator=(const char *str)
    {
        clear();
        strncpy(buffer_, str, sizeof(buffer_));
        return *this;
    }

private:
    char buffer_[MaxSize];
    const char *delimeter_;
};