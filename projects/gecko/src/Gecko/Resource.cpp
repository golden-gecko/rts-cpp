#include "Gecko/Resource.hpp"

#include "Gecko/Configuration.hpp"

namespace Gecko
{
    Resource::Resource(const std::string& name) :
        m_name(name)
    {

    }

    ConfigurationPtr Resource::serialize() const
    {
        auto configuration = std::make_shared<Configuration>();

        configuration->set("name", m_name);

        configuration->set("current", m_current);
        configuration->set("maximal", m_maximal);
        configuration->set("consumption", m_consumption);
        configuration->set("production", m_production);

        configuration->set("need_deposit", m_need_deposit);
        configuration->set("need_storage", m_need_storage);

        configuration->set("deposit_range", m_deposit_range);
        configuration->set("storage_range", m_storage_range);

        configuration->set("priority", m_priority);

        return configuration;
    }

    void Resource::deserialize(const ConfigurationPtr& configuration)
    {
        m_current = configuration->get_float("current", 0.0f);
        m_maximal = configuration->get_float("maximal", 0.0f);
        m_consumption = configuration->get_float("consumption", 0.0f);
        m_production = configuration->get_float("production", 0.0f);

        m_need_deposit = configuration->get_bool("need_deposit", false);
        m_need_storage = configuration->get_bool("need_storage", false);

        m_deposit_range = configuration->get_float("deposit_range", 0.0f);
        m_storage_range = configuration->get_float("storage_range", 0.0f);

        m_priority = configuration->get_float("priority", Settings::Game::ResourcePriority);
    }

    void Resource::update(float time)
    {
        // TODO: Implement resource degradation and expansion (spreading).
    }

    float Resource::add(float value)
    {
        if (m_current + value > m_maximal)
        {
            value = m_maximal - m_current;
        }

        m_current += value;
        m_current = std::clamp(m_current, 0.0f, m_maximal);

        return value;
    }

    float Resource::remove(float value)
    {
        if (value > m_current)
        {
            value = m_current;
        }

        m_current -= value;
        m_current = std::clamp(m_current, 0.0f, m_maximal);

        return value;
    }
}
