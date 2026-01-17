#include "Gecko/Skills/Skill.hpp"

#include "Gecko/Configuration.hpp"

namespace Gecko
{
    ConfigurationPtr Skill::serialize() const
    {
        auto configuration = std::make_shared<Configuration>();

        configuration->set("name", m_name);
        configuration->set("refresh_timer", m_refresh_timer.serialize());
        configuration->set("activation_count", m_activation_count);

        return configuration;
    }

    void Skill::deserialize(const ConfigurationPtr& configuration)
    {
        m_name = configuration->get_string("name");

        if (configuration->has_member("refresh_timer"))
        {
            m_refresh_timer.deserialize(configuration->get_child("refresh_timer"));
        }

        m_activation_count = configuration->get_int<std::uint16_t>("activation_count", 0);
    }

    void Skill::update(float time)
    {
        m_refresh_timer.update(time);
    }

    Skill::Skill(const Skill& other) :
        base_type(other)
    {
        m_name = other.m_name;
        m_refresh_timer = other.m_refresh_timer;
        m_activation_count = other.m_activation_count;
    }
}
