#include "Gecko/Components/Component.hpp"

#include "Gecko/Configuration.hpp"
#include "Gecko/Objects/Object.hpp"

namespace Gecko
{
    ConfigurationPtr Component::serialize() const
    {
        auto configuration = base_type::serialize();

        configuration->set("name", m_name);
        configuration->set("health", m_health_timer.serialize());

        return configuration;
    }

    void Component::deserialize(const ConfigurationPtr& configuration)
    {
        base_type::deserialize(configuration);

        m_name = configuration->get_string("name");

        if (configuration->has_member("health"))
        {
            m_health_timer.deserialize(configuration->get_child("health"));
        }
    }

    void Component::update(float time)
    {
    }

    void Component::set_position(const Ogre::Vector3& position)
    {
    }

    void Component::set_visible(bool visible)
    {
    }
}
