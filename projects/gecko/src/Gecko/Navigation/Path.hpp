#pragma once

#include "Gecko/Navigation/Coordinate.hpp"

namespace Gecko::Navigation
{
    class Path
    {
    public:
        typedef std::deque<Coordinate> Points;

    public:
        explicit Path() = default;
        explicit Path(const Coordinate& start, const Coordinate& end, const Points& points);

    public:
        auto& get_points()
        {
            return m_points;
        }

        const auto& get_points() const
        {
            return m_points;
        }

    public:
        void set_points(const Points& points)
        {
            m_points = points;
        }

    public:
        std::string to_string() const;

    private:
        Coordinate m_start;
        Coordinate m_end;

        Points m_points;
    };
}
