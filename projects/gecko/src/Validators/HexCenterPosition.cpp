#include "Validators/HexCenterPosition.hpp"

#include "Layers/Layer.hpp"

namespace Gecko
{
    HexCenterPosition::HexCenterPosition(const Layer& layer) :
        base_type(layer)
    {
    }

    Ogre::Vector3 HexCenterPosition::validate(const Ogre::Vector3& position, const Ogre::Vector3& direction) const
    {
        // TODO: Implement.
        return Ogre::Vector3::ZERO;
    }
}
