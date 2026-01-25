#include "Gecko/Navigation/Coordinate.hpp"

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
        return Utils::Convert::to_string(x) + ":" + Utils::Convert::to_string(y) + ":" + Utils::Convert::to_string(z);
    }

    bool Coordinate::operator==(const Coordinate& other) const
    {
        return x == other.x && y == other.y && z == other.z;
    }

    bool Coordinate::operator!=(const Coordinate& other) const
    {
        return !(*(this) == other);
    }

    bool Coordinate::operator<(const Coordinate& other) const
    {
        return std::tie(x, y, z) < std::tie(other.x, other.y, other.z);
    }

    bool Coordinate::operator>(const Coordinate& other) const
    {
        return std::tie(x, y, z) > std::tie(other.x, other.y, other.z);
    }
}
