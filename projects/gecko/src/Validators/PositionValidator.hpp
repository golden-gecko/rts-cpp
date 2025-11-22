#pragma once

namespace Gecko
{
    class PositionValidator
    {
    public:
        explicit PositionValidator(const Layer& layer);

        virtual ~PositionValidator() = default;

    public:
        virtual Ogre::Vector3 validate(const Ogre::Vector3& position, const Ogre::Vector3& direction = Ogre::Vector3::ZERO) const;

    protected:
        const Layer& m_layer;
    };
}
