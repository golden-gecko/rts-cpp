#include "Validators/PositionValidator.hpp"

namespace Gecko
{
    PositionValidator::PositionValidator(const Layer& layer) :
        m_layer(layer)
    {
    }

    Ogre::Vector3 PositionValidator::validate(const Ogre::Vector3& position, const Ogre::Vector3& direction) const
    {
        return position;
    }
}
