#include "Components/Drive.hpp"

#include "Configuration.hpp"

namespace Gecko
{
    Drive* Drive::create(Drive* memory, const std::shared_ptr<Configuration>& configuration)
    {
        auto component = new (memory) Drive();

        component->deserialize(configuration);

        return component;
    }

    Drive::Drive(const Drive& other) :
        base_type(other)
    {
        speed = other.speed;
        distance_driven = other.distance_driven;
    }

    std::shared_ptr<Configuration> Drive::serialize() const
    {
        auto configuration = base_type::serialize();

        configuration->set("speed", speed);
        configuration->set("distance_driven", distance_driven);

        return configuration;
    }

    void Drive::deserialize(const std::shared_ptr<Configuration>& configuration)
    {
        base_type::deserialize(configuration);

        speed = configuration->get_float("speed", 0.0f);
        speed = configuration->get_float("distance_driven", 0.0f);
    }
}
