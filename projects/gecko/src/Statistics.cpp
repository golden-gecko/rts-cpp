#include "Statistics.hpp"

template<> Gecko::Statistics* Ogre::Singleton<Gecko::Statistics>::msSingleton = nullptr;

namespace Gecko
{
    void Statistics::add(const std::string& name, float value)
    {
        if (m_values.find(name) != m_values.end())
        {
            m_values[name] += value;
        }
        else
        {
            m_values.emplace(name, value);
        }
    }
}
