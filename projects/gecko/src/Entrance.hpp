#pragma once

namespace Gecko
{
    class Entrance
    {
    public:
        Entrance(const Ogre::Vector3& position, const Ogre::Vector3& direction);

    public:
        const auto& get_direction() const
        {
            return m_direction;
        }

        const auto& get_position() const
        {
            return m_position;
        }

    protected:
        Ogre::Vector3 m_position = Ogre::Vector3::ZERO;
        Ogre::Vector3 m_direction = Ogre::Vector3::ZERO;
    };
}
