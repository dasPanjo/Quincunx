#pragma once

#include <string_view>

namespace Penjin
{
    enum class ErrorCode : int
    {
#define X(name, code, description) name = code,
#include "ErrorCodes.def"
#undef X
    };

    constexpr std::string_view errorDescription(ErrorCode code)
    {
        switch (code)
        {
#define X(name, code, description) \
            case ErrorCode::name: return description;
#include "ErrorCodes.def"
#undef X
        }

        return "Unknown error";
    }

    constexpr std::string_view errorDescription(int code)
    {
        switch (code)
        {
#define X(name, code, description) \
            case code: return description;
#include "ErrorCodes.def"
#undef X
        }

        return "Unknown error";
    }
}