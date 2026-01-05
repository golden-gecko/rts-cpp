#pragma once

#include "Gecko/Cache.hpp"
#include "Gecko/Path.hpp"

namespace Gecko
{
    class Search
    {
    public:
        enum class Type :
            std::uint8_t
        {
            AvoidOccupied,
            DirectLine,
            FollowRoads
        };

    public:
        explicit Search(const Layer& layer);

        virtual ~Search() = default;

    public:
        virtual std::optional<Path::Points> get_path(const Ogre::Vector3& from, const Ogre::Vector3& to) = 0;

    protected:
        const Layer& m_layer;

        Cache<std::pair<Coordinate, Coordinate>, Path::Points> m_cache; // TODO: Make private.

        void save_cache(const std::pair<Coordinate, Coordinate>& key, const Path::Points& points);
    };
}
