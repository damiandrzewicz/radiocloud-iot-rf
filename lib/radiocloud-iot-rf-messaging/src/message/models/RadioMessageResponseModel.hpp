#pragma once
#include "RadioMessageModel.hpp"

struct RadioMessageResponseModel : protected RadioMessageModel
{
    enum class Result
    {
        OK,
        ERROR
    };

    Result result;
};