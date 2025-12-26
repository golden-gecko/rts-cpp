#pragma once

#include "Gecko/Log.hpp"

namespace Gecko
{
    class Exception : public std::runtime_error
    {
    public:
        Exception(const std::string& message) :
            std::runtime_error(message.c_str())
        {
            // L_ERROR << message;
        }
    };
}
