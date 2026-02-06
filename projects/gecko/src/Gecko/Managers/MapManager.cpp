#include "Gecko/Managers/MapManager.hpp"

#include "Gecko/Configuration.hpp"
#include "Gecko/Log.hpp"
#include "Gecko/Managers/ConfigurationManager.hpp"
#include "Gecko/Maps/Map.hpp"
#include "Gecko/Utils/Time.hpp"

Gecko::MapManager* Ogre::Singleton<Gecko::MapManager>::msSingleton = nullptr;

namespace Gecko
{
    void MapManager::update(float time)
    {
        auto update = [](Map& map, float time)
        {
            map.update(time);
        };

        iterate(std::bind(update, std::placeholders::_1, time));
    }

    void MapManager::init(const ConfigurationPtr& configuration)
    {
        L_TIME("MapManager::init()");

        auto max_size = configuration->get_int<std::size_t>("memory.maps");
        auto& map_manager = MapManager::getSingleton();

        for (const auto& [name, configuration] : ConfigurationManager::getSingleton())
        {
            L_INFO << "Loading '" << name << "' configuration.";

            auto type = configuration->get_string("type", "");

            if (type == "Map")
            {
                auto function = static_cast<Map*(*)(Map* memory, const ConfigurationPtr&)>(&Map::create);
                auto factory = std::bind(function, std::placeholders::_1, configuration);

                map_manager.register_type<Map>(name, factory);
                map_manager.allocate(name, max_size);
            }
        }
    }

    void MapManager::deinit()
    {
        deallocate();
    }
}
