#include "Gecko/Objects/Factory.hpp"

namespace Gecko
{
    Factory* Factory::create(Factory* memory, const ConfigurationPtr& configuration)
    {
        auto map = new (memory) Factory();

        map->deserialize(configuration);

        return map;
    }

    /*
    Factory::Factory(const std::string& configuration_name) :
        base_type(configuration_name)
    {
        auto configuration = ConfigurationManager::getSingleton().get(configuration_name);
        auto configurations_configuration = configuration->get_child("configurations");

        for (auto i = configurations_configuration.begin(); i != configurations_configuration.end(); i++)
        {
            configurations.emplace(i->asString());
        }

        auto usage_configuration = configuration->get_child_optional("usage");

        if (usage_configuration)
        {
            for (auto i = usage_configuration.get().begin(); i != usage_configuration.get().end(); i++)
            {
                usage.emplace(i.key(), i->.second.get<float>(""));
            }
        }
    }
    */

    /*
    ObjectAttributes Factory::to_string() const
    {
        auto attributes = base_type::to_string();

        attributes["Attributes"]["Created objects"] = created_objects;
        attributes["Attributes"]["Rally point"] = Convert::to_string(rally_point);
        attributes["Attributes"]["Spent resources"] = Convert::to_string(spent_resources);

        return attributes;
    }
    */
}
