#include "Gecko/Skills/Repair.hpp"

#include "Gecko/Configuration.hpp"
#include "Gecko/Utils/Utils.hpp"

namespace Gecko
{
    Repair* Repair::create(Repair* memory, const std::shared_ptr<Configuration>& configuration)
    {
        auto component = new (memory) Repair();

        component->deserialize(configuration);

        return component;
    }

    std::shared_ptr<Configuration> Repair::serialize() const
    {
        auto configuration = std::make_shared<Configuration>();

        configuration->set("percentage", m_percentage);
        configuration->set("value", m_value);

        return configuration;
    }

    void Repair::deserialize(const std::shared_ptr<Configuration>& configuration)
    {
        base_type::deserialize(configuration);

        m_percentage = configuration->get_float("percentage", 0.0f);
        m_value = configuration->get_float("value", 0.0f);
    }

    void Repair::activate(const Id& id)
    {
        if (m_refresh_timer)
        {
            // TODO: Implement.

            ++m_activation_count;

            m_refresh_timer.reset();
        }
    }

    void Repair::activate(const Ogre::Vector3& position)
    {
        if (m_refresh_timer)
        {
            // TODO: Implement.

            ++m_activation_count;

            m_refresh_timer.reset();
        }
    }
}
