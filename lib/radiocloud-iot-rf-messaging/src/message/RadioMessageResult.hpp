#pragma once

enum RadioMessageBuildResult
{
    OK = 0,
    VERIFY_ERROR
};

enum RadioMessageParseResult
{
    OK = 0,
    VERIFY_ERROR,
    MESSAGE_TYPE_MISSING,
    MESSAGE_DIRECTION_MISSING,
    MODEL_MISMATCH,
};