#include "Gecko/Skills/Buff.hpp"

#include "Gecko/Configuration.hpp"

namespace Gecko
{
    ConfigurationPtr Buff::serialize() const
    {
        return std::make_shared<Configuration>();
    }

    void Buff::deserialize(const ConfigurationPtr& configuration)
    {
        base_type::deserialize(configuration);
    }

    void Buff::activate(const Id& id)
    {
        if (m_refresh_timer)
        {
            ++m_activation_count;

            m_refresh_timer.reset();
        }
    }

    void Buff::activate(const Ogre::Vector3& position)
    {
        if (m_refresh_timer)
        {
            ++m_activation_count;

            m_refresh_timer.reset();
        }
    }

    Buff* Buff::create(Buff* memory, const ConfigurationPtr& configuration)
    {
        auto component = new (memory) Buff();

        component->deserialize(configuration);

        return component;
    }
}
