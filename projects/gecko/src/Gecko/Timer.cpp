#include "Gecko/Timer.hpp"

#include "Gecko/Configuration.hpp"

namespace Gecko
{
    Timer::Timer(float maximal) :
        maximal(maximal)
    {
    }

    Timer::Timer(const Timer& other)
    {
        current = other.current;
        maximal = other.maximal;
    }

    ConfigurationPtr Timer::serialize() const
    {
        auto configuration = std::make_shared<Configuration>();

        configuration->set("current", current);
        configuration->set("maximal", maximal);

        return configuration;
    }

    void Timer::deserialize(const ConfigurationPtr& configuration)
    {
        current = configuration->get_float("current", 0.0f);
        maximal = configuration->get_float("maximal", std::numeric_limits<decltype(maximal)>::max());
    }

    Timer& Timer::update(float time)
    {
        current = std::clamp(current + time, 0.0f, maximal);

        return *this;
    }
}
