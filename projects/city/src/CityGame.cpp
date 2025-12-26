#include "CityGame.hpp"

#include "Citizen.hpp"
#include "CityBuilding.hpp"
#include "CityMap.hpp"
#include "Depot.hpp"
#include "Log.hpp"
#include "Managers/ConfigurationManager.hpp"
#include "Managers/MapManager.hpp"
#include "Managers/ObjectManager.hpp"
#include "Utils/Time.hpp"

template<> City::Game* Ogre::Singleton<City::Game>::msSingleton = nullptr;

namespace City
{
    Game::Game(std::shared_ptr<Gecko::Configuration> configuration) :
        base_type(configuration)
    {
    }

    void Game::init_maps(std::size_t max_size)
    {
        SCOPED_TIME("City::Game::init_maps()");

        base_type::init_maps(max_size);

        auto& map_manager = Gecko::MapManager::getSingleton();

        for (const auto& [name, configuration] : Gecko::ConfigurationManager::getSingleton())
        {
            // L_INFO << "Loading '" << name << "' configuration.";

            auto type = configuration->get_string("type", "");

            if (type == "City")
            {
                auto function = static_cast<Map*(*)(Map* memory, std::shared_ptr<Gecko::Configuration>)>(&Map::create);
                auto factory = std::bind(function, std::placeholders::_1, configuration);

                map_manager.register_type<Map>(name, factory);
                map_manager.allocate(name, max_size);            }
        }
    }

    void Game::init_objects(std::size_t max_size)
    {
        SCOPED_TIME("City::Game::init_objects()");

        base_type::init_objects(max_size);

        auto& object_manager = Gecko::ObjectManager::getSingleton();

        for (const auto& [name, configuration] : Gecko::ConfigurationManager::getSingleton())
        {
            // L_INFO << "Loading '" << name << "' configuration.";

            auto type = configuration->get_string("type", "");

            if (type == "CityBuilding")
            {
                auto factory = std::bind(CityBuilding::create, std::placeholders::_1, configuration);

                object_manager.register_type<CityBuilding>(name, factory);
                object_manager.allocate(name, max_size);
            }
            else if (type == "Citizen")
            {
                auto factory = std::bind(Citizen::create, std::placeholders::_1, configuration);

                object_manager.register_type<Citizen>(name, factory);
                object_manager.allocate(name, max_size);
            }
            else if (type == "Depot")
            {
                auto factory = std::bind(Depot::create, std::placeholders::_1, configuration);

                object_manager.register_type<Depot>(name, factory);
                object_manager.allocate(name, max_size);
            }
        }
    }
}
