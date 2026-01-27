#include "Gecko/Validators/RoadPosition.hpp"

#include "Gecko/Layers/Layer.hpp"

namespace Gecko
{
    RoadPosition::RoadPosition(const Layer& layer) :
        base_type(layer)
    {
    }

    Ogre::Vector3 RoadPosition::validate(const Ogre::Vector3& position, const Ogre::Vector3& direction) const
    {
        const Navigation::Coordinate& index = m_layer.get_index(position);
        const Ogre::Vector3& scale = m_layer.get_scale();

        return Ogre::Vector3(
            index.x * scale.x + scale.x / 2.0f - direction.z * scale.x / 4.0f, position.y,
            index.z * scale.z + scale.z / 2.0f + direction.x * scale.z / 4.0f
        );
    }
}
