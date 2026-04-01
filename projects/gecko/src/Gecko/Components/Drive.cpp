#include "Gecko/Components/Drive.hpp"

#include "Gecko/Configuration.hpp"

namespace Gecko
{
    ConfigurationPtr Drive::serialize() const
    {
        ConfigurationPtr configuration = base_type::serialize();

        configuration->set("speed", m_speed);
        configuration->set("distance_driven", m_distance_driven);

        return configuration;
    }

    void Drive::deserialize(const ConfigurationPtr& configuration)
    {
        base_type::deserialize(configuration);

        m_speed = configuration->get_float("speed", 0.0f);
        m_distance_driven = configuration->get_float("distance_driven", 0.0f);
    }

    Drive* Drive::create(Drive* memory, const ConfigurationPtr& configuration, const ScenePtr& scene)
    {
        auto component = new (memory) Drive();

        component->deserialize(configuration);

        return component;
    }
}
