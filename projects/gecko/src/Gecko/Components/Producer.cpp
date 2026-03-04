#include "Gecko/Components/Producer.hpp"

#include "Gecko/Configuration.hpp"
#include "Gecko/Containers/Processes.hpp"

namespace Gecko
{
    ConfigurationPtr Producer::serialize() const
    {
        ConfigurationPtr configuration = base_type::serialize();

        configuration->set("processes", m_processes->serialize());
        configuration->set("produce_time", m_produce_time);

        return configuration;
    }

    void Producer::deserialize(const ConfigurationPtr& configuration)
    {
        base_type::deserialize(configuration);

        m_processes->deserialize(configuration->get_child("processes"));
        m_produce_time = configuration->get_float("produce_time", 0.0f);
    }

    void Producer::update(float time)
    {
        // time, get_id(), get_position(), get_resources()
        // m_processes->update(time);
    }

    Producer* Producer::create(Producer* memory, const ConfigurationPtr& configuration, const ScenePtr& scene)
    {
        auto component = new (memory) Producer();

        component->deserialize(configuration);

        return component;
    }

    Producer::Producer()
    {
        m_processes = std::make_shared<Processes>();
    }

    Producer::Producer(const Producer& other) :
        m_produce_time(other.m_produce_time)
    {
        m_processes = std::make_shared<Processes>(*other.m_processes);
    }
}
