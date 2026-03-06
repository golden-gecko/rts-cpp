#include "Gecko/Containers/Processes.hpp"

#include "Gecko/Configuration.hpp"

namespace Gecko
{
    ConfigurationPtr Processes::serialize() const
    {
        auto configuration = std::make_shared<Configuration>();

        for (const auto& [name, process] : m_items)
        {
            configuration->append(process.serialize());
        }

        return configuration;
    }

    void Processes::deserialize(const ConfigurationPtr& configuration)
    {
        clear();

        for (auto i = configuration->begin(); i != configuration->end(); i++)
        {
            auto name = i.key().asString();
            auto process = Process(name);

            process.deserialize(std::make_shared<Configuration>(*i));

            m_items.emplace(name, process);
        }
    }

    void Processes::update(float time, const Id& id, const Ogre::Vector3& position, const ResourcesPtr& resources)
    {
        for (auto& [name, process] : m_items)
        {
            process.update(time, id, position, resources);
        }
    }
}
