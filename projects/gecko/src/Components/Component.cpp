#include "Components/Component.hpp"

#include "Configuration.hpp"
#include "Objects/Object.hpp"

namespace Gecko
{
    std::shared_ptr<Configuration> Component::serialize() const
    {
        auto configuration = base_type::serialize();

        configuration->set("name", m_name);
        configuration->set("health", m_health_timer.serialize());

        return configuration;
    }

    void Component::deserialize(const std::shared_ptr<Configuration>& configuration)
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
