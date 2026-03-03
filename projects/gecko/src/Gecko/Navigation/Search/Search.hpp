#pragma once

#include "Gecko/Cache.hpp"
#include "Gecko/Navigation/Path.hpp"

namespace Gecko::Navigation
{
    class Search
    {
    public:
        explicit Search(const Layer& layer);

        virtual ~Search() = default;

    public:
        virtual std::optional<Path::Points> get_path(const Ogre::Vector3& from, const Ogre::Vector3& to) = 0;

    protected:
        const Layer& m_layer;

        Cache<std::pair<Coordinate, Coordinate>, Path::Points> m_cache;

        void save_cache(const std::pair<Coordinate, Coordinate>& key, const Path::Points& points);
    };
}
