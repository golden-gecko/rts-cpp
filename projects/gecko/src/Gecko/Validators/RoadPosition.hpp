#pragma once

#include "Gecko/Validators/PositionValidator.hpp"

namespace Gecko
{
    class RoadPosition :
        public PositionValidator
    {
    private:
        using base_type = PositionValidator;

    // From PositionValidator.
    public:
        Ogre::Vector3 validate(const Ogre::Vector3& position, const Ogre::Vector3& direction = Ogre::Vector3::ZERO) const override;

    // RoadPosition.
    public:
        explicit RoadPosition(const Layer& layer);
    };
}
