#include "Gecko/Navigation/Path.hpp"

namespace Gecko
{
    Path::Path(const Coordinate& start, const Coordinate& end, const Points& points) :
        m_start(start),
        m_end(end),
        m_points(points)
    {
    }

    std::string Path::to_string() const
    {
        std::stringstream stream;

        stream << m_start.to_string();
        stream << " ";
        stream << m_end.to_string();
        stream << " ";
        stream << m_points.size();

        return stream.str();
    }
}
