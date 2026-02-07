#pragma once

namespace Gecko
{
    class Exception :
        public std::runtime_error
    {
    public:
        explicit Exception(const std::string& message);
    };
}
