#pragma once

#include "Gecko/Validators/PositionValidator.hpp"

namespace Gecko
{
    class SquareCenterPosition :
        public PositionValidator
    {
    private:
        using base_type = PositionValidator;

    public:
        // From PositionValidator.
        Ogre::Vector3 validate(const Ogre::Vector3& position, const Ogre::Vector3& direction = Ogre::Vector3::ZERO) const override;

    public:
        explicit SquareCenterPosition(const Layer& layer);
    };
}
