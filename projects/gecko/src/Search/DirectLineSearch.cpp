#include "Search/DirectLineSearch.hpp"

#include "Layers/Layer.hpp"

namespace Gecko
{
    DirectLineSearch::DirectLineSearch(const Layer& layer) :
        base_type(layer)
    {
    }

    std::optional<Path::Points> DirectLineSearch::get_path(const Ogre::Vector3& from, const Ogre::Vector3& to)
    {
        auto from_index = m_layer.get_index(from);
        auto to_index = m_layer.get_index(to);

        if (from_index == to_index)
        {
            return Path::Points();
        }

        return Path::Points { to_index };
    }
}
