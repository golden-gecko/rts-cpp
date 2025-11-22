#include "Components/Shield.hpp"

#include "Configuration.hpp"

namespace Gecko
{
    Shield* Shield::create(Shield* memory, const std::shared_ptr<Configuration>& configuration)
    {
        auto component = new (memory) Shield();

        component->deserialize(configuration);

        return component;
    }

    Shield::Shield(const Shield& other) :
        base_type(other)
    {
        defence = other.defence;
    }

    std::shared_ptr<Configuration> Shield::serialize() const
    {
        auto configuration = base_type::serialize();

        configuration->set("defence", defence);

        return configuration;
    }

    void Shield::deserialize(const std::shared_ptr<Configuration>& configuration)
    {
        base_type::deserialize(configuration);

        defence = configuration->get_float("defence", 0.0f);
    }
}
