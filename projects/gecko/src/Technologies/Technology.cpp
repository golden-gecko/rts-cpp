#include "Technologies/Technology.hpp"

#include "Configuration.hpp"

namespace Gecko
{
    std::shared_ptr<Configuration> Technology::serialize() const
    {
        auto configuration = std::make_shared<Configuration>();

        configuration->set("name", name);
        configuration->set("costs", costs);
        configuration->set("obsoletes", name);
        configuration->set("unlocks", unlocks);
        configuration->set("research_timer", research_timer.serialize());
        configuration->set("in_research", in_research);
        configuration->set("locked", locked);

        return configuration;
    }

    void Technology::deserialize(const std::shared_ptr<Configuration>& configuration)
    {
        name = configuration->get_string("name");
        costs = configuration->get_map<std::string, float>("costs");
        obsoletes = configuration->get_string_array<std::set<std::string>>("obsoletes", {});
        unlocks = configuration->get_string_array<std::set<std::string>>("unlocks", {});

        if (configuration->has_member("research_timer"))
        {
            research_timer.deserialize(configuration->get_child("research_timer"));
        }

        in_research = configuration->get_bool("in_research", false);
        locked = configuration->get_bool("locked", true);
    }

    void Technology::update(float time)
    {
        if (in_research && research_timer.update(time))
        {
            unlock();
        }
    }
}
