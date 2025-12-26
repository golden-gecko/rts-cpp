#pragma once

#include "Gecko/Search/Search.hpp"

namespace Gecko
{
    class FollowRoadsSearch : public Search
    {
    private:
        using base_type = Search;

    public:
        explicit FollowRoadsSearch(const Layer& layer);

    public:
        std::optional<Path::Points> get_path(const Ogre::Vector3& from, const Ogre::Vector3& to) override;
    };
}
