#include "Gecko/Statistics.hpp"

Gecko::Statistics* Ogre::Singleton<Gecko::Statistics>::msSingleton = nullptr;

namespace Gecko
{
    void Statistics::add(const std::string& name, float value)
    {
        m_values[name] += value;
    }
}
