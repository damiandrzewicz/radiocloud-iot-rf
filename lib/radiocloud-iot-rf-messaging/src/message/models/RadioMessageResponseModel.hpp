#pragma once
#include "RadioMessageModel.hpp"

struct RadioMessageResponseModel : public RadioMessageModel
{
    enum class Result
    {
        OK,
        ERROR
    };

    Result result = Result::OK;
};