#include "Gecko/Timer.hpp"

#include "Gecko/Configuration.hpp"

namespace Gecko
{
    Timer::Timer(float max) :
        m_max(max)
    {
    }

    Timer::Timer(const Timer& other)
    {
        m_current = other.m_current;
        m_max = other.m_max;
    }

    ConfigurationPtr Timer::serialize() const
    {
        ConfigurationPtr configuration = std::make_shared<Configuration>();

        configuration->set("current", m_current);
        configuration->set("max", m_max);

        return configuration;
    }

    void Timer::deserialize(const ConfigurationPtr& configuration)
    {
        m_current = configuration->get_float("current", 0.0f);
        m_max = configuration->get_float("max", std::numeric_limits<decltype(m_max)>::max());
    }

    Timer& Timer::update(float time)
    {
        m_current = std::clamp(m_current + time, 0.0f, m_max);

        return *this;
    }
}
