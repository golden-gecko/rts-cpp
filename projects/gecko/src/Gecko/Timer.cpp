#include "Gecko/Timer.hpp"

#include "Gecko/Configuration.hpp"

namespace Gecko
{
    Timer::Timer(float maximal) :
        m_maximal(maximal)
    {
    }

    Timer::Timer(const Timer& other)
    {
        m_current = other.m_current;
        m_maximal = other.m_maximal;
    }

    ConfigurationPtr Timer::serialize() const
    {
        auto configuration = std::make_shared<Configuration>();

        configuration->set("current", m_current);
        configuration->set("maximal", m_maximal);

        return configuration;
    }

    void Timer::deserialize(const ConfigurationPtr& configuration)
    {
        m_current = configuration->get_float("current", 0.0f);
        m_maximal = configuration->get_float("maximal", std::numeric_limits<decltype(m_maximal)>::max());
    }

    Timer& Timer::update(float time)
    {
        m_current = std::clamp(m_current + time, 0.0f, m_maximal);

        return *this;
    }
}
