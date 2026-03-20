#include "Gecko/Managers/MapManager.hpp"

#include "Gecko/Configuration.hpp"
#include "Gecko/Log.hpp"
#include "Gecko/Managers/ConfigurationManager.hpp"
#include "Gecko/Maps/Map.hpp"
#include "Gecko/Utils/Time.hpp"

Gecko::MapManager* Ogre::Singleton<Gecko::MapManager>::msSingleton = nullptr;

namespace Gecko
{
    void MapManager::init()
    {
        L_TIME("MapManager::init()");

        std::uint64_t max_size = m_configuration->get_int<std::uint64_t>("memory.maps");

        for (const auto& [name, configuration] : ConfigurationManager::getSingleton())
        {
            L_INFO << "Loading '" << name << "' configuration.";

            if (std::optional<std::string> type = configuration->get_string_optional("type"))
            {
                if (type == TO_STRING(Map))
                {
                    register_type<Map>(name, std::bind(Map::create, std::placeholders::_1, configuration, m_scene));
                }

                allocate(name, max_size);
            }
        }
    }

    void MapManager::deinit()
    {
        unregister_all();
    }
    
    void MapManager::update(float time)
    {
        auto update = [time](Map& map)
        {
            map.update(time);
        };

        iterate(std::bind(update, std::placeholders::_1));
    }

    MapManager::MapManager(const ConfigurationPtr& configuration, const ScenePtr& scene) :
        m_configuration(configuration),
        m_scene(scene)
    {
    }
}
