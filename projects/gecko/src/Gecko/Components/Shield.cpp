#include "Gecko/Components/Shield.hpp"

#include "Gecko/Configuration.hpp"

namespace Gecko
{
    Shield* Shield::create(Shield* memory, const ConfigurationPtr& configuration)
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

    ConfigurationPtr Shield::serialize() const
    {
        auto configuration = base_type::serialize();

        configuration->set("defence", defence);

        return configuration;
    }

    void Shield::deserialize(const ConfigurationPtr& configuration)
    {
        base_type::deserialize(configuration);

        defence = configuration->get_float("defence", 0.0f);
    }
}
