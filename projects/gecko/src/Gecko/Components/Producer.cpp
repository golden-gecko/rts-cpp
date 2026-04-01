#include "Gecko/Components/Producer.hpp"

#include "Gecko/Configuration.hpp"
#include "Gecko/Containers/Processes.hpp"
#include "Gecko/Process.hpp"

namespace Gecko
{
    ConfigurationPtr Producer::serialize() const
    {
        ConfigurationPtr configuration = base_type::serialize();

        configuration->set("produce_time", m_produce_time.serialize());

        return configuration;
    }

    void Producer::deserialize(const ConfigurationPtr& configuration)
    {
        base_type::deserialize(configuration);

        if (configuration->has_member("produce_time"))
        {
            m_produce_time.deserialize(configuration->get_child("produce_time"));
        }
    }

    Producer* Producer::create(Producer* memory, const ConfigurationPtr& configuration, const ScenePtr& scene)
    {
        auto component = new (memory) Producer();

        component->deserialize(configuration);

        return component;
    }
}
