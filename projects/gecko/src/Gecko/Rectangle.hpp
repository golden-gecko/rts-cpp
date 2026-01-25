#pragma once

#include "Gecko/Navigation/Coordinate.hpp"

namespace Gecko
{
    struct Rectangle
    {
    public:
        Coordinate start;
        Coordinate end;

    public:
        explicit Rectangle() = default;
        explicit Rectangle(const Coordinate& start, const Coordinate& end);
        explicit Rectangle(Index start_x, Index start_z, Index end_x, Index end_z);

    public:
        std::string to_string() const;

    public:
        bool operator==(const Rectangle& other) const
        {
            return start == other.start && end == other.end;
        }

        bool operator!=(const Rectangle& other) const
        {
            return !(*this == other);
        }

        bool operator<(const Rectangle& other) const
        {
            return start < other.start && end < other.end;
        }
    };
}
