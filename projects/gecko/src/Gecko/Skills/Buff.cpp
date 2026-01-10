#include "Gecko/Skills/Buff.hpp"

#include "Gecko/Configuration.hpp"

namespace Gecko
{
    Buff* Buff::create(Buff* memory, const ConfigurationPtr& configuration)
    {
        auto component = new (memory) Buff();

        component->deserialize(configuration);

        return component;
    }

    ConfigurationPtr Buff::serialize() const
    {
        auto configuration = std::make_shared<Configuration>();

        // TODO: Implement.

        return configuration;
    }

    void Buff::deserialize(const ConfigurationPtr& configuration)
    {
        base_type::deserialize(configuration);

        // TODO: Implement.
    }

    void Buff::activate(const Id& id)
    {
        if (m_refresh_timer)
        {
            // TODO: Implement.

            ++m_activation_count;

            m_refresh_timer.reset();
        }
    }

    void Buff::activate(const Ogre::Vector3& position)
    {
        if (m_refresh_timer)
        {
            // TODO: Implement.

            ++m_activation_count;

            m_refresh_timer.reset();
        }
    }
}
