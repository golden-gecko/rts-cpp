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
        configuration->set("max", m_max);

        configuration->set("consumption", m_consumption);
        configuration->set("production", m_production);

        configuration->set("need_deposit", m_need_deposit);
        configuration->set("need_storage", m_need_storage);

        configuration->set("deposit_range", m_deposit_range);
        configuration->set("storage_range", m_storage_range);

        return configuration;
    }

    void Resource::deserialize(const ConfigurationPtr& configuration)
    {
        m_current = configuration->get_int<std::uint64_t>("current", 0);
        m_max = configuration->get_int<std::uint64_t>("max", 0);

        m_consumption = configuration->get_float("consumption", 0.0f);
        m_production = configuration->get_float("production", 0.0f);

        m_need_deposit = configuration->get_bool("need_deposit", false);
        m_need_storage = configuration->get_bool("need_storage", false);

        m_deposit_range = configuration->get_float("deposit_range", 0.0f);
        m_storage_range = configuration->get_float("storage_range", 0.0f);
    }

    void Resource::update(float time)
    {
        // TODO: Implement resource degradation and expansion (spreading).
    }

    std::uint64_t Resource::add(std::uint64_t value)
    {
        if (m_current + value > m_max)
        {
            value = m_max - m_current;
        }

        m_current += value;
        m_current = std::clamp<std::uint64_t>(m_current, 0, m_max);

        return value;
    }

    std::uint64_t Resource::remove(std::uint64_t value)
    {
        if (value > m_current)
        {
            value = m_current;
        }

        m_current -= value;
        m_current = std::clamp<std::uint64_t>(m_current, 0, m_max);

        return value;
    }
}
