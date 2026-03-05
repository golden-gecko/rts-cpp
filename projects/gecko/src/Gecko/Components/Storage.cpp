#include "Gecko/Components/Storage.hpp"

#include "Gecko/Configuration.hpp"
#include "Gecko/Containers/Resources.hpp"

namespace Gecko
{
    ConfigurationPtr Storage::serialize() const
    {
        ConfigurationPtr configuration = base_type::serialize();

        configuration->set("load_time", m_load_time);
        configuration->set("unload_time", m_unload_time);

        return configuration;
    }

    void Storage::deserialize(const ConfigurationPtr& configuration)
    {
        base_type::deserialize(configuration);

        m_load_time = configuration->get_float("load_time", 0.0f);
        m_unload_time = configuration->get_float("unload_time", 0.0f);
    }

    Storage* Storage::create(Storage* memory, const ConfigurationPtr& configuration, const ScenePtr& scene)
    {
        auto component = new (memory) Storage();

        component->deserialize(configuration);

        return component;
    }

    Storage::Storage()
    {
    }

    Storage::Storage(const Storage& other) :
        base_type(other),
        m_load_time(other.m_load_time),
        m_unload_time(other.m_unload_time)
    {
    }
}
