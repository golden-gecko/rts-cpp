#include "Utils/Random.hpp"

namespace Gecko::Utils::Random
{
    Ogre::Vector3 get_position(const Ogre::Vector3& min, const Ogre::Vector3& max)
    {
        return Ogre::Vector3(get_real(min.x, max.x), get_real(min.y, max.y), get_real(min.z, max.z));
    }
}
