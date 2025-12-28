#include "Gecko/Utils/Math.hpp"

namespace Gecko::Utils::Math
{
    bool is_power_2(std::size_t number)
    {
        if (number < 1)
        {
            return false;
        }

        return static_cast<std::size_t>(std::ceil(std::log2(number)))
            == static_cast<std::size_t>(std::floor(std::log2(number)));
    }
}
