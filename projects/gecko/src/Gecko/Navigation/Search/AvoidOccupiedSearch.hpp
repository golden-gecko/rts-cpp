#pragma once

#include "Gecko/Navigation/Search/Search.hpp"

namespace Gecko
{
    class AvoidOccupiedSearch :
        public Search
    {
    private:
        using base_type = Search;

    public:
        // From Search.
        std::optional<Path::Points> get_path(const Ogre::Vector3& from, const Ogre::Vector3& to) override;

    public:
        explicit AvoidOccupiedSearch(const Layer& layer);
    };
}
