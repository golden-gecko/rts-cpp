#include "Gecko/Technologies/Technology.hpp"

#include "Gecko/Configuration.hpp"

namespace Gecko
{
    ConfigurationPtr Technology::serialize() const
    {
        auto configuration = std::make_shared<Configuration>();

        configuration->set("name", m_name);
        configuration->set("costs", m_costs);
        configuration->set("obsoletes", m_name);
        configuration->set("unlocks", m_unlocks);
        configuration->set("research_timer", m_research_timer.serialize());
        configuration->set("in_research", m_in_research);
        configuration->set("locked", m_locked);

        return configuration;
    }

    void Technology::deserialize(const ConfigurationPtr& configuration)
    {
        m_name = configuration->get_string("name");
        m_costs = configuration->get_map<std::string, float>("costs");
        m_obsoletes = configuration->get_string_array<std::set<std::string>>("obsoletes", {});
        m_unlocks = configuration->get_string_array<std::set<std::string>>("unlocks", {});

        if (configuration->has_member("research_timer"))
        {
            m_research_timer.deserialize(configuration->get_child("research_timer"));
        }

        m_in_research = configuration->get_bool("in_research", false);
        m_locked = configuration->get_bool("locked", true);
    }

    void Technology::update(float time)
    {
        if (m_in_research && m_research_timer.update(time))
        {
            unlock();
        }
    }
}
