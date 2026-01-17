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
        m_defence = other.m_defence;
    }

    ConfigurationPtr Shield::serialize() const
    {
        auto configuration = base_type::serialize();

        configuration->set("defence", m_defence);

        return configuration;
    }

    void Shield::deserialize(const ConfigurationPtr& configuration)
    {
        base_type::deserialize(configuration);

        m_defence = configuration->get_float("defence", 0.0f);
    }
}
