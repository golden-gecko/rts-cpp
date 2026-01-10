#pragma once

namespace Gecko
{
    class Coordinate
    {
    public:
        Index x = 0;
        Index y = 0;
        Index z = 0;

        explicit Coordinate(Index x = 0, Index y = 0, Index z = 0);

    public:
        float distance(const Coordinate& other) const;

    public:
        std::string to_string() const;

    public:
        bool operator==(const Coordinate& other) const
        {
            return x == other.x && y == other.y && z == other.z;
        }

        bool operator!=(const Coordinate& other) const
        {
            return !(*(this) == other);
        }

        bool operator<(const Coordinate& other) const
        {
            return std::tie(x, y, z) < std::tie(other.x, other.y, other.z);
        }

        bool operator>(const Coordinate& other) const
        {
            return std::tie(x, y, z) > std::tie(other.x, other.y, other.z);
        }
    };
}
