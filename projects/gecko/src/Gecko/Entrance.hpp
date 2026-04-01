#pragma once

namespace Gecko
{
    class Entrance
    {
    public:
        explicit Entrance(const Ogre::Vector3& position, const Ogre::Vector3& direction);

    public:
        const Ogre::Vector3& get_direction() const
        {
            return m_direction;
        }

        const Ogre::Vector3& get_position() const
        {
            return m_position;
        }

    protected:
        Ogre::Vector3 m_position = Ogre::Vector3::ZERO;
        Ogre::Vector3 m_direction = Ogre::Vector3::ZERO;
    };
}
