#pragma once

enum RadioMessageResult
{
    OK = 0,
    ERROR,
    VERIFY_ERROR,

    // Build

    // Parse
    P_MESSAGE_TYPE_MISSING,
    P_MESSAGE_DIRECTION_MISSING,
    P_MODEL_MISMATCH,
    P_MISSING_RESULT,
    P_MISSING_PARAM
};