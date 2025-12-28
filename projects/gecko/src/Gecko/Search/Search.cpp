#include "Gecko/Search/Search.hpp"

namespace Gecko
{
    Search::Search(const Layer& layer) :
        m_layer(layer)
    {
    }

    void Search::save_cache(const std::pair<Coordinate, Coordinate>& key, const Path::Points& points)
    {
        auto reversed_key = key;
        auto reversed_points = points;

        std::swap(reversed_key.first, reversed_key.second);
        std::ranges::reverse(reversed_points);

        m_cache.set(key, points);
        m_cache.set(reversed_key, reversed_points);
    }
}
