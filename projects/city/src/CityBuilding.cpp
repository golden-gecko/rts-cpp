#include "CityBuilding.hpp"

#include <Gecko/Games/Game.hpp>
#include <Gecko/Validators/RoadPosition.hpp>

#include "Citizen.hpp"

namespace City
{
    CityBuilding::CityBuilding()
    {
    }

    /*
    CityBuilding::CityBuilding(const std::string& configuration_name) :
        base_type(configuration_name)
    {
        auto configuration = Gecko::ConfigurationManager::getSingleton().get(Gecko::ConfigurationType::Object, configuration_name);

        industrial = configuration->get_bool("industrial", false);
        residential = configuration->get_bool("residential", false);

        max_residents = configuration->get_int("max_residents", 0);
        max_workers = configuration->get_int("max_workers", 0);
    }
    */

    CityBuilding::CityBuilding(const CityBuilding& other) :
        base_type(other)
    {
        industrial = other.industrial;
        residential = other.residential;

        max_residents = other.max_residents;
        max_workers = other.max_workers;
    }

    Gecko::Entrance CityBuilding::get_entrance() const
    {
        /*
        const auto& position = get_position();
        const auto& scale = get_scale();

        // TODO: Remove active map.
        // TODO: Remove hardcoded value.
        const auto& layer = Gecko::Game::getSingleton().get_active_map()->get_layer("Terrain");
        auto validator = Gecko::RoadPosition(*layer.get());

        const auto& terrain = layer;
        auto terrain_scale = layer->get_scale();

        int sX = (position.x - scale.x / 2) / terrain_scale.x;
        int sZ = (position.z - scale.z / 2) / terrain_scale.z;
        int eX = (position.x + scale.x / 2) / terrain_scale.x - 1;
        int eZ = (position.z + scale.z / 2) / terrain_scale.z - 1;

        // TODO: Remove hardcoded value.
        auto data_layer = layer->get_data_layer("Roads");

        for (int x = sX; x <= eX; x++)
        {
            for (int z = sZ; z <= eZ; z++)
            {
                int indices[4][2] =
                {
                    {  0, -1 },
                    {  1,  0 },
                    {  0,  1 },
                    { -1,  0 }
                };

                for (int i = 0; i < 4; i++)
                {
                    auto data_layer_value = data_layer->get_data(x + indices[i][0], z + indices[i][1]);

                    // TODO: Remove hardcoded value.
                    if (data_layer_value[0] + data_layer_value[1] + data_layer_value[2] == 0 && data_layer_value[3] == 255)
                    {
                        return Gecko::Entrance(
                            Ogre::Vector3(
                                (x + indices[i][0]) * terrain_scale.x + terrain_scale.x / 2.0f - indices[i][0] * terrain_scale.x / 4.0f,
                                0.0f,
                                (z + indices[i][1]) * terrain_scale.z + terrain_scale.z / 2.0f - indices[i][1] * terrain_scale.z / 4.0f),
                            Ogre::Vector3(
                                -indices[i][0],
                                0.0f,
                                -indices[i][1]
                            )
                        );
                    }
                }
            }
        }

        L_WARNING << "CityBuilding has no entrance.";
        */

        return Gecko::Entrance(Ogre::Vector3::ZERO, Ogre::Vector3::ZERO);
    }

    std::size_t CityBuilding::get_present_residents() const
    {
        std::size_t count = 0;

        /*
        for (const auto& resident_id : residents)
        {
            auto resident = Gecko::ObjectManager::getSingleton().get<Citizen>(resident_id);

            if (resident && resident->get_state() == Gecko::State::Value::None)
            {
                ++count;
            }
        }
        */

        return count;
    }

    std::size_t CityBuilding::get_present_workers() const
    {
        std::size_t count = 0;

        /*
        for (const auto& worker_id : workers)
        {
            auto worker = Gecko::ObjectManager::getSingleton().get<Citizen>(worker_id);

            if (worker && worker->get_state() == Gecko::State::Value::Working)
            {
                ++count;
            }
        }
        */

        return count;
    }

    float CityBuilding::get_inhabitancy() const
    {
        if (max_residents == 0)
        {
            return 0.0f;
        }

        return static_cast<float>(get_present_residents()) / static_cast<float>(max_residents);
    }

    float CityBuilding::get_work_efficiency() const
    {
        if (max_workers == 0)
        {
            return 0.0f;
        }

        return static_cast<float>(get_present_workers()) / static_cast<float>(max_workers);
    }

    /*
    Gecko::ObjectAttributes CityBuilding::to_string() const
    {
        auto attributes = base_type::to_string();

        if (is_residential())
        {
            attributes["Attributes"]["Inhabitancy"]
                = Gecko::Convert::to_string(get_inhabitancy() * 100.0) + "%";

            attributes["Attributes"]["Residents"]
                = Gecko::Convert::to_string(get_present_residents())
                + "/"
                + Gecko::Convert::to_string(residents.size())
                + "/"
                + Gecko::Convert::to_string(max_residents);
        }

        if (is_industrial())
        {
            attributes["Attributes"]["Work efficiency"]
                = Gecko::Convert::to_string(get_work_efficiency() * 100.0) + "%";

            attributes["Attributes"]["Workers"]
                = Gecko::Convert::to_string(get_present_workers())
                + "/"
                + Gecko::Convert::to_string(workers.size())
                + "/"
                + Gecko::Convert::to_string(max_workers);
        }

        return attributes;
    }
    */

    /*
    Gecko::Rectangle CityBuilding::get_occupied_space() const
    {
        TODO: Fix.
        const auto& terrain = Gecko::Game::getSingleton().get_active_map()->get_terrain();
        auto terrain_scale = terrain.get_scale();

        auto position = get_position();
        auto index = terrain.get_index(position);
        auto scale = get_scale();

        auto start = position - scale / 2.0f;
        auto end = position + scale / 2.0f - terrain_scale;

        auto start_index = terrain.get_index(start);
        auto end_index = terrain.get_index(end);

        return Gecko::Rectangle(start_index, end_index);

        return Gecko::Rectangle(0, 0, 0, 0);
    }
    */

    void CityBuilding::set_position(const Ogre::Vector3& position, bool validate)
    {
        base_type::set_position(position);

        /*
        TODO: Fix.
        // Get previous position.
        auto previous_space = get_occupied_space();

        // Set position.
        base_type::set_position(position);

        // Get current position.
        auto current_space = get_occupied_space();

        // Update terrain layer.
        auto& terrain = Gecko::Game::getSingleton().get_active_map()->get_terrain();

        if (previous_space != current_space)
        {
            terrain.get_layer(Gecko::LayerType::Value::Occupied)->as<Gecko::TerrainLayerOccupied>()->substract(previous_space, 1);
            terrain.get_layer(Gecko::LayerType::Value::Occupied)->as<Gecko::TerrainLayerOccupied>()->add(current_space, 1);
        }
        */
    }

    void CityBuilding::update_resources(float time)
    {
        // TODO: Move resource efficiency production to engine.
        // Produce resources with speed related to number of workers present in the building.
        base_type::update_resources(time); // * get_work_efficiency());
    }
}
