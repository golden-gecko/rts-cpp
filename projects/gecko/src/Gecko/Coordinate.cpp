#include "Gecko/Coordinate.hpp"

#include "Gecko/Utils/Convert.hpp"

namespace Gecko
{
    Coordinate::Coordinate(Index x, Index y, Index z) :
        x(x),
        y(y),
        z(z)
    {
    }

    float Coordinate::distance(const Coordinate& other) const
    {
        return std::sqrtf((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y) + (z - other.z) * (z - other.z));
    }

    std::string Coordinate::to_string() const
    {
        using namespace Utils;

        return Convert::to_string(x) + ":" + Convert::to_string(y) + ":" + Convert::to_string(z);
    }
}
