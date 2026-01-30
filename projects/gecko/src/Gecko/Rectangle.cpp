#include "Gecko/Rectangle.hpp"

namespace Gecko
{
    Rectangle::Rectangle(const Navigation::Coordinate& start, const Navigation::Coordinate& end) :
        start(start),
        end(end)
    {
    }

    Rectangle::Rectangle(Index start_x, Index start_z, Index end_x, Index end_z) :
        start(start_x, start_z),
        end(end_x, end_z)
    {
    }

    std::string Rectangle::to_string() const
    {
        return start.to_string() + " " + end.to_string();
    }
}
