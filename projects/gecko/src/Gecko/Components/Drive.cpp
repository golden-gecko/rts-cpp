#include "Gecko/Components/Drive.hpp"

#include "Gecko/Configuration.hpp"

namespace Gecko
{
    Drive* Drive::create(Drive* memory, const ConfigurationPtr& configuration)
    {
        auto component = new (memory) Drive();

        component->deserialize(configuration);

        return component;
    }

    Drive::Drive(const Drive& other) :
        base_type(other)
    {
        m_speed = other.m_speed;
        m_distance_driven = other.m_distance_driven;
    }

    ConfigurationPtr Drive::serialize() const
    {
        auto configuration = base_type::serialize();

        configuration->set("speed", m_speed);
        configuration->set("distance_driven", m_distance_driven);

        return configuration;
    }

    void Drive::deserialize(const ConfigurationPtr& configuration)
    {
        base_type::deserialize(configuration);

        m_speed = configuration->get_float("speed", 0.0f);
        m_speed = configuration->get_float("distance_driven", 0.0f);
    }
}
