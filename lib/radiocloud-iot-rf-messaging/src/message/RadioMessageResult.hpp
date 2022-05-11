#pragma once

enum RadioMessageResult
{
    OK = 0,
    ERROR,
    EMPTY_BUFFER,
    MODEL_MALFORMED,
    MESSAGE_MALFORMED,
    MISSING_RESULT
};