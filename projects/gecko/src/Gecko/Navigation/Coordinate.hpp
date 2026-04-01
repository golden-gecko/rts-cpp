#pragma once

namespace Gecko::Navigation
{
    class Coordinate
    {
    public:
        Index x = 0;
        Index y = 0;
        Index z = 0;

        Coordinate(const Index& x = 0, const Index& y = 0, const Index& z = 0);

    public:
        bool operator==(const Coordinate& other) const;
        bool operator!=(const Coordinate& other) const;

        bool operator<(const Coordinate& other) const;
        bool operator>(const Coordinate& other) const;

    public:
        float distance(const Coordinate& other) const;

        std::string to_string() const;
    };
}
