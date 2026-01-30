#pragma once

namespace Gecko
{
    class Area
    {
    public:
        explicit Area() = default;
        explicit Area(const Ogre::Vector3& start, const Ogre::Vector3& end);

    public:
        const Ogre::Vector3& get_end() const
        {
            return m_end;
        }

        const Ogre::Vector3& get_start() const
        {
            return m_start;
        }

        // TODO: Rename this method or Object::get_size() to Object::get_area().
        Ogre::Vector3 get_size() const;

    protected:
        Ogre::Vector3 m_start = Ogre::Vector3::ZERO;
        Ogre::Vector3 m_end   = Ogre::Vector3::ZERO;
    };
}
