#include "Gecko/Rectangle.hpp"

namespace Gecko
{
    Rectangle::Rectangle(const Navigation::Coordinate& start, const Navigation::Coordinate& end) :
        m_start(start),
        m_end(end)
    {
    }

    Rectangle::Rectangle(const Index& start_x, const Index& start_z, const Index& end_x, const Index& end_z) :
        m_start(start_x, start_z),
        m_end(end_x, end_z)
    {
    }

    bool Rectangle::operator==(const Rectangle& other) const
    {
        return std::tie(m_start, m_end) == std::tie(other.m_start, other.m_end);
    }

    bool Rectangle::operator!=(const Rectangle& other) const
    {
        return !(*this == other);
    }

    bool Rectangle::operator<(const Rectangle& other) const
    {
        return std::tie(m_start, m_end) < std::tie(other.m_start, other.m_end);
    }

    bool Rectangle::operator>(const Rectangle& other) const
    {
        return std::tie(m_start, m_end) > std::tie(other.m_start, other.m_end);
    }

    std::string Rectangle::to_string() const
    {
        return m_start.to_string() + " " + m_end.to_string();
    }
}
