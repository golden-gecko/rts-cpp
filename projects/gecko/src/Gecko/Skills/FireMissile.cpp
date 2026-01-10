#include "Gecko/Skills/FireMissile.hpp"

#include "Gecko/Configuration.hpp"
#include "Gecko/Objects/Object.hpp"
#include "Gecko/Utils/Utils.hpp"

namespace Gecko
{
    FireMissile* FireMissile::create(FireMissile* memory, const ConfigurationPtr& configuration)
    {
        auto component = new (memory) FireMissile();

        component->deserialize(configuration);

        return component;
    }

    ConfigurationPtr FireMissile::serialize() const
    {
        auto configuration = std::make_shared<Configuration>();

        configuration->set("missile.configuration", m_missile_configuration);

        return configuration;
    }

    void FireMissile::deserialize(const ConfigurationPtr& configuration)
    {
        base_type::deserialize(configuration);

        m_missile_configuration = configuration->get_string("missile.configuration");
    }

    void FireMissile::activate(const Id& id)
    {
        if (m_refresh_timer)
        {
            Utils::fire_missile(*m_owner, m_missile_configuration, m_owner->get_position(), m_owner->get_direction());

            ++m_activation_count;

            m_refresh_timer.reset();
        }
    }

    void FireMissile::activate(const Ogre::Vector3& position)
    {
        if (m_refresh_timer)
        {
            // TODO: Remove + 2.0f.
            auto position = m_owner->get_position() + Ogre::Vector3::UNIT_Y * 2.0f;

            Utils::fire_missile(*m_owner, m_missile_configuration, position, m_owner->get_direction());

            ++m_activation_count;

            m_refresh_timer.reset();
        }
    }
}
