#include "Gecko/Validators/SquareCenterPosition.hpp"

#include "Gecko/Layers/Layer.hpp"

namespace Gecko
{
    SquareCenterPosition::SquareCenterPosition(const Layer& layer) :
        base_type(layer)
    {
    }

    Ogre::Vector3 SquareCenterPosition::validate(const Ogre::Vector3& position, const Ogre::Vector3& direction) const
    {
        const auto& index = m_layer.get_index(position);
        const auto& scale = m_layer.get_grid_scale();

        return Ogre::Vector3(
            index.x * scale.x + scale.x * 0.5f, position.y,
            index.z * scale.z + scale.z * 0.5f
        );
    }
}
