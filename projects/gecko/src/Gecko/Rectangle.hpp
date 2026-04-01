#pragma once

#include "Gecko/Navigation/Coordinate.hpp"

namespace Gecko
{
    struct Rectangle
    {
    public:
        Rectangle(const Navigation::Coordinate& start, const Navigation::Coordinate& end);
        Rectangle(const Index& start_x, const Index& start_z, const Index& end_x, const Index& end_z);

    public:
        bool operator==(const Rectangle& other) const;
        bool operator!=(const Rectangle& other) const;

        bool operator<(const Rectangle& other) const;
        bool operator>(const Rectangle& other) const;

    public:
        std::string to_string() const;

    private:
        Navigation::Coordinate m_start;
        Navigation::Coordinate m_end;
    };
}
