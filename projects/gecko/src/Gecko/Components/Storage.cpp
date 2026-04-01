#include "Gecko/Components/Storage.hpp"

#include "Gecko/Configuration.hpp"
#include "Gecko/Containers/Resources.hpp"

namespace Gecko
{
    ConfigurationPtr Storage::serialize() const
    {
        ConfigurationPtr configuration = base_type::serialize();

        configuration->set("load_time", m_load_time.serialize());
        configuration->set("unload_time", m_unload_time.serialize());

        return configuration;
    }

    void Storage::deserialize(const ConfigurationPtr& configuration)
    {
        base_type::deserialize(configuration);

        if (configuration->has_member("load_time"))
        {
            m_load_time.deserialize(configuration->get_child("load_time"));
        }

        if (configuration->has_member("unload_time"))
        {
            m_unload_time.deserialize(configuration->get_child("unload_time"));
        }
    }

    Storage* Storage::create(Storage* memory, const ConfigurationPtr& configuration, const ScenePtr& scene)
    {
        auto component = new (memory) Storage();

        component->deserialize(configuration);

        return component;
    }
}
