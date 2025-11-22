#include "Components/Armour.hpp"

#include "Configuration.hpp"

namespace Gecko
{
    Armour* Armour::create(Armour* memory, const std::shared_ptr<Configuration>& configuration)
    {
        auto component = new (memory) Armour();

        component->deserialize(configuration);

        return component;
    }

    Armour::Armour(const Armour& other) :
        base_type(other)
    {
        m_defence = other.m_defence;
    }

    std::shared_ptr<Configuration> Armour::serialize() const
    {
        auto configuration = base_type::serialize();

        configuration->set("m_defence", m_defence);

        return configuration;
    }

    void Armour::deserialize(const std::shared_ptr<Configuration>& configuration)
    {
        base_type::deserialize(configuration);

        m_defence = configuration->get_float("defence", 0.0f);
    }
}
