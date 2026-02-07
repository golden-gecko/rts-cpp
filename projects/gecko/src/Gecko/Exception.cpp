#include "Gecko/Exception.hpp"

#include "Gecko/Log.hpp"

namespace Gecko
{
    Exception::Exception(const std::string& message) :
        std::runtime_error(message.c_str())
    {
        L_ERROR << message;
    }
}
