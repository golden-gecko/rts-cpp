#pragma once

#include "Gecko/Navigation/Coordinate.hpp"

namespace Gecko::Navigation
{
    class Path
    {
    public:
        using Points = std::deque<Coordinate>;

    public:
        explicit Path() = default;
        explicit Path(const Coordinate& start, const Coordinate& end, const Points& points);
        explicit Path(const Points& points);

    public:
        std::string to_string() const;

    public:
        Points& get_points()
        {
            return m_points;
        }

        const Points& get_points() const
        {
            return m_points;
        }

    public:
        void set_points(const Points& points)
        {
            m_points = points;
        }

    private:
        Coordinate m_start;
        Coordinate m_end;

        Points m_points;
    };
}
