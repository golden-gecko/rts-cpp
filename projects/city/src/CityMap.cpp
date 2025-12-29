#include <Gecko/Configuration.hpp>
#include <Gecko/DataLayers/DataLayer.hpp>
#include <Gecko/Exception.hpp>
#include <Gecko/Games/Game.hpp>
#include <Gecko/Layers/Layer.hpp>
#include <Gecko/Log.hpp>
#include <Gecko/Managers/ObjectManager.hpp>
#include <Gecko/Objects/Vehicle.hpp>
#include <Gecko/Utils/Random.hpp>
#include <Gecko/Validators/RoadPosition.hpp>

#include "Citizen.hpp"
#include "CityBuilding.hpp"
#include "CityMap.hpp"

namespace City
{
    Map* Map::create(std::shared_ptr<Gecko::Configuration> configuration)
    {
        auto map = new Map();

        map->deserialize(configuration);

        return map;
    }

    Map* Map::create(Map* memory, std::shared_ptr<Gecko::Configuration> configuration)
    {
        auto map = new (memory) Map();

        map->deserialize(configuration);

        return map;
    }

    void Map::init_objects()
    {
        base_type::init_objects();

        init_buildings();
    }

    void Map::init_buildings()
    {
        typedef std::vector<bool> Visited;

        auto layer = get_layer("Terrain");

        if (!layer)
        {
            throw Gecko::Exception("Map " + get_name() + " has no Terrain layer.");
        }

        auto data_layer = layer->get_data_layer("Type");

        if (!layer)
        {
            throw Gecko::Exception("Layer " + layer->get_name() + " has no Type data layer.");
        }

        auto size = layer->get_size();

        std::vector<Visited> processed(size, Visited(size, false));

        for (std::size_t z = 0; z < size; ++z)
        {
            for (std::size_t x = 0; x < size; ++x)
            {
                if (processed[x][z] == true)
                {
                    continue;
                }

                auto type = data_layer->get_data(x, z);

                // TODO: Refactor. Create data layer class.
                auto is_industrial = true; // (type[0] == 255 && type[1] == 255 && type[2] == 0);
                auto is_residential = true; // (type[0] == 0 && type[1] == 255 && type[2] == 0);

                if (is_industrial || is_residential)
                {
                    create_building(layer, is_industrial, is_residential, x, z, 1, 1);

                    /*
                    std::size_t xx;

                    for (xx = x; xx < size; xx++)
                    {
                        if (processed[xx][z] == true)
                        {
                            break;
                        }

                        if (data_layer->get_data(xx, z) != type)
                        {
                            break;
                        }
                    }

                    std::size_t zz;

                    for (zz = z; zz < size; zz++)
                    {
                        if (processed[x][zz] == true)
                        {
                            break;
                        }

                        if (data_layer->get_data(x, zz) != type)
                        {
                            break;
                        }
                    }

                    for (std::size_t j = z; j < zz; ++j)
                    {
                        for (std::size_t i = x; i < xx; i++)
                        {
                            processed[i][j] = true;
                        }
                    }

                    create_building(layer, is_industrial, is_residential, x, z, xx - x, zz - z);
                    */
                }
            }
        }
    }

    void Map::init_citizens(CityBuilding* home)
    {
        for (std::size_t i = 0; i < home->get_max_residents(); i++)
        {
            auto object = Gecko::ObjectManager::getSingleton().create("citizen");

            if (object == nullptr)
            {
                L_WARNING << "Failed to create citizen.";

                continue;
            }

            auto citizen = dynamic_cast<Citizen*>(object);

            if (citizen == nullptr)
            {
                L_WARNING << "Failed to create citizen.";

                continue;
            }

            // Assign to player.
            citizen->set_player_id(home->get_player_id());

            // Assign to home.
            citizen->set_home_id(home->get_id());

            // Place at entrance.
            auto entrance = home->get_entrance();

            // if (entrance)
            {
                // TODO: Remove hardcoded value.
                auto layer = get_layer("Terrain");
                auto validator = Gecko::RoadPosition(*layer.get());

                /*
                citizen->set_position(
                    validator.validate_position(
                        entrance.get_position(),
                        // TODO: Test. Not working correctly.
                        entrance.get_direction().crossProduct(Ogre::Vector3::NEGATIVE_UNIT_Y)
                    )
                );
                */
            }

            // TODO: Refactor.
            //std::string configurations[] =
            //{
            //    "car",
            //    "bus",
            //    "truck"
            //};

            //object = Gecko::ObjectManager::getSingleton().create(configurations[Gecko::Random::get_int(0, 2)]);

            //if (object == nullptr)
            //{
            //    L_WARNING << "Failed to create vehicle";

            //    continue;
            //}

            //auto vehicle = Gecko::ObjectManager::getSingleton().get<Vehicle>(object->get_id());

            //if (vehicle == nullptr)
            //{
            //    L_WARNING << "Failed to create vehicle";

            //    continue;
            //}
        }
    }

    void Map::init_vehicles(CityBuilding* home)
    {
        // TODO: Read from configuration.
        for (std::size_t i = 0; i < 1; i++)
        {
            auto object = Gecko::ObjectManager::getSingleton().create("truck");

            if (object == nullptr)
            {
                L_WARNING << "Failed to create truck.";

                continue;
            }

            auto vehicle = dynamic_cast<Gecko::Vehicle*>(object);

            if (vehicle == nullptr)
            {
                L_WARNING << "Failed to create truck.";

                continue;
            }

            // Assign to player.
            vehicle->set_player_id(home->get_player_id());

            // Assign to home.
            // citizen->set_home_id(home->get_id());

            // Place at entrance.
            auto entrance = home->get_entrance();

            // if (entrance)
            {
                // TODO: Remove active map.
                // TODO: Remove hardcoded value.
                auto layer = get_layer("Terrain");
                auto validator = Gecko::RoadPosition(*layer.get());

                /*
                vehicle->set_position(
                    validator.validate_position(
                        entrance.get_position(),
                        // TODO: Test. Not working correctly.
                        entrance.get_direction().crossProduct(Ogre::Vector3::NEGATIVE_UNIT_Y)
                    )
                );
                */
            }
        }
    }

    void Map::create_building(std::shared_ptr<Gecko::Layer> layer, bool is_industrial, bool is_residential, std::size_t x, std::size_t z, std::size_t size_x, std::size_t size_z)
    {
        CityBuilding* building = nullptr;

        /*
        // TODO: Remove or fix.
        if (0 && is_industrial)
        {
            std::vector<std::string> industrial_configurations;

            for (const auto& configuration : Gecko::ConfigurationManager::getSingleton())
            {
                if (configuration.second->get_bool("industrial", false))
                {
                    industrial_configurations.emplace_back(configuration.first);
                }
            }

            auto object = Gecko::ObjectManager::getSingleton().create(
                industrial_configurations[
                    Gecko::Random::get_int<std::size_t>(0, industrial_configurations.size() - 1)
                ]
            );

            building = dynamic_cast<CityBuilding*>(object);
        }
        else if (is_residential)
        {
            std::vector<std::string> residential_configurations;

            for (const auto& configuration : Gecko::ConfigurationManager::getSingleton())
            {
                if (configuration.second->get_bool("residential", false))
                {
                    residential_configurations.emplace_back(configuration.first);
                }
            }

            auto object = Gecko::ObjectManager::getSingleton().create(
                residential_configurations[
                    Gecko::Random::get_int<std::size_t>(0, residential_configurations.size() - 1)
                ]
            );

            building = dynamic_cast<CityBuilding*>(object);
        }

        if (building != nullptr)
        {
            auto y = Gecko::Random::get_real(3.0f, 6.0f);

            auto scale = Ogre::Vector3(
                size_x * layer->get_scale().x,
                y,
                size_z * layer->get_scale().z
            );

            auto position = Ogre::Vector3(
                x * layer->get_scale().x + size_x * layer->get_scale().x / 2.0f,
                0.0f,
                z * layer->get_scale().z + size_z * layer->get_scale().z / 2.0f
            );

            building->set_scale(scale);
            building->set_position(position);

            // TODO: Do not use active player ID.
            building->set_player_id(Gecko::Game::getSingleton().get_active_player_id());

            init_citizens(building);
            init_vehicles(building);
        }
        */
    }
}
