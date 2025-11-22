#include "Maps/Map.hpp"

#include "Cameras/FreeCamera.hpp"
#include "Cameras/MinimapCamera.hpp"
#include "Cameras/ObjectFollowCamera.hpp"
#include "Cameras/TopDownCamera.hpp"
#include "Configuration.hpp"
#include "Exception.hpp"
#include "Games/Game.hpp"
#include "Layers/SquareLayer.hpp"
#include "Managers/ObjectManager.hpp"
#include "Managers/OrderManager.hpp"
#include "Managers/PlayerManager.hpp"
#include "Objects/Object.hpp"
#include "Players/Player.hpp"
#include "Season.hpp"
#include "Window.hpp"

namespace Gecko
{
    Map* Map::create()
    {
        return new Map();
    }

    Map* Map::create(Map* memory)
    {
        return new (memory) Map();
    }

    Map* Map::create(const std::shared_ptr<Configuration>& configuration)
    {
        auto map = new Map();

        map->deserialize(configuration);

        return map;
    }

    Map* Map::create(Map* memory, const std::shared_ptr<Configuration>& configuration)
    {
        auto map = new (memory) Map();

        map->deserialize(configuration);

        return map;
    }

    Map::Map(const Map& other) :
        base_type(other)
    {
        m_name = other.m_name;

        for (const auto& [name, layer] : other.m_layers)
        {
            // m_layers.emplace(name, std::make_shared<SquareLayer>(*layer)); // TODO: Fix. Add clone methods to each object.
        }

        for (const auto& [name, season] : other.m_seasons)
        {
            m_seasons.emplace(name, std::make_shared<Season>(*season));
        }
    }

    void Map::init()
    {
        base_type::init();

        init_cameras();
        init_layers();
        init_seasons();

        init_players();
        init_objects();

        set_visible(true);
    }

    void Map::deinit()
    {
        base_type::deinit();

        deinit_cameras();
        deinit_layers();
        deinit_seasons();

        m_layers.clear();
        m_seasons.clear();

        PlayerManager::getSingleton().destroy_all();
        OrderManager::getSingleton().destroy_all();

        set_visible(false);
    }

    std::shared_ptr<Configuration> Map::serialize() const
    {
        auto configuration = base_type::serialize();

        configuration->set("name", m_name);

        return configuration;
    }

    void Map::deserialize(const std::shared_ptr<Configuration>& configuration)
    {
        base_type::deserialize(configuration);

        m_name = get_configuration()->get_string("name");
    }

    void Map::update(float time)
    {
        for (auto& [name, camera] : m_cameras)
        {
            camera->update(time);
        }

        for (auto& [name, layer] : m_layers)
        {
            layer->update(time);
        }

        for (auto& [name, season] : m_seasons)
        {
            season->update(time);
        }
    }

    bool Map::is_position_valid(const Ogre::Vector3& position) const
    {
        return get_layer("Terrain")->is_position_valid(position); // TODO: Hardcoded layer name.
    }

    void Map::show_data_layer(const std::string& layer_name, const std::string& data_layer_name)
    {
        // TODO: Check if layer exists.
        get_layer(layer_name)->show_data_layer(data_layer_name);
    }

    std::shared_ptr<Layer> Map::get_layer(const std::string& name) const
    {
        auto layer = m_layers.find(name);

        if (layer == m_layers.end())
        {
            throw Exception("Map '" + get_name() + "' has no '" + name + "' layer.");
        }

        return layer->second;
    }

    std::shared_ptr<Season> Map::get_season(const std::string& name) const
    {
        auto season = m_seasons.find(name);

        if (season == m_seasons.end())
        {
            throw Exception("Map '" + get_name() + "' has no '" + name + "' season.");
        }

        return season->second;
    }

    void Map::set_visible(bool visible)
    {
        for (auto& [name, layer] : m_layers)
        {
            layer->set_visible(visible);
        }
    }

    void Map::init_cameras()
    {
        auto cameras_configuration = m_configuration->get_child("cameras");

        for (auto i = cameras_configuration->begin(); i != cameras_configuration->end(); ++i)
        {
            auto camera_configuration = std::make_shared<Configuration>(*i);

            auto camera_name = camera_configuration->get_string("name");
            auto camera_type = camera_configuration->get_string("type");

            if (camera_type == "Free")
            {
                m_cameras.emplace(camera_name, std::make_shared<FreeCamera>(
                    Game::getSingleton().get_root(), Game::getSingleton().get_scene_manager(), camera_name, camera_configuration
                ));
            }
            else if (camera_type == "Minimap")
            {
                m_cameras.emplace(camera_name, std::make_shared<MinimapCamera>(
                    Game::getSingleton().get_root(), Game::getSingleton().get_scene_manager(), camera_name, camera_configuration
                ));
            }
            else if (camera_type == "ObjectFollow")
            {
                m_cameras.emplace(camera_name, std::make_shared<ObjectFollowCamera>(
                    Game::getSingleton().get_root(), Game::getSingleton().get_scene_manager(), camera_name, camera_configuration
                ));
            }
            else if (camera_type == "TopDown")
            {
                m_cameras.emplace(camera_name, std::make_shared<TopDownCamera>(
                    Game::getSingleton().get_root(), Game::getSingleton().get_scene_manager(), camera_name, camera_configuration
                ));
            }
            else
            {
                throw Exception("Unknown camera type '" + camera_type + "'.");
            }
        }
    }

    void Map::init_layers()
    {
        m_layers.clear();

        auto layers_configuration = m_configuration->get_child_optional("layers");

        if (layers_configuration)
        {
            for (auto i = layers_configuration.value()->begin(); i != layers_configuration.value()->end(); ++i)
            {
                auto name = i.key().asString();

                m_layers.emplace(name, std::make_shared<SquareLayer>(this, name, Configuration(*i)));
            }

            for (const auto& [name, layer] : m_layers)
            {
                layer->init();
            }
        }
    }

    void Map::init_objects()
    {
        auto objects_configuration = m_configuration->get_child_optional("objects");

        if (objects_configuration)
        {
            for (const auto& i : *(*(objects_configuration)))
            {
                auto object_configuration = Configuration(i);
                auto configuration = object_configuration.get_string("configuration");
                auto object = ObjectManager::getSingleton().create(configuration);

                if (object == nullptr)
                {
                    throw Exception("Failed to create '" + configuration + "' object.");
                }

                object->set_owner(this);
                object->init();

                auto player_name = object_configuration.get_string("player");
                auto position = object_configuration.get_vector3("position", Ogre::Vector3::ZERO);

                for (const auto& [id, player] : PlayerManager::getSingleton())
                {
                    if (player->get_configuration()->get_name() == player_name)
                    {
                        object->set_player_id(id);

                        break;
                    }
                }

                object->set_position(position);
            }
        }
    }

    void Map::init_players()
    {
        auto players_configuration = m_configuration->get_child_optional("players");

        if (players_configuration)
        {
            auto active_player = m_configuration->get_string("active_player");

            for (const auto& i : *(*(players_configuration)))
            {
                auto player_configuration = Configuration(i);
                auto configuration = player_configuration.get_string("configuration");

                for (const auto& [id, player] : PlayerManager::getSingleton())
                {
                    if (player->get_configuration()->get_name() == configuration)
                    {
                        throw Exception("Player '" + configuration + "' already exists.");
                    }
                }

                auto player = PlayerManager::getSingleton().create(configuration);

                if (player == nullptr)
                {
                    throw Exception("Failed to create '" + configuration + "' player.");
                }

                player->init();

                if (configuration == active_player)
                {
                    Game::getSingleton().set_active_player_id(player->get_id());
                }
            }
        }
    }

    void Map::init_seasons()
    {
        m_seasons.clear();

        auto seasons_configuration = m_configuration->get_child_optional("seasons");

        if (seasons_configuration)
        {
            for (auto i = seasons_configuration.value()->begin(); i != seasons_configuration.value()->end(); ++i)
            {
                auto name = i.key().asString();

                m_seasons.emplace(name, std::make_shared<Season>(name, Configuration(*i)));
            }

            for (const auto& [name, season] : m_seasons)
            {
                season->init();
            }
        }
    }

    void Map::deinit_cameras()
    {
        m_cameras.clear();
    }

    void Map::deinit_layers()
    {
        for (const auto& [name, layer] : m_layers)
        {
            layer->deinit();
        }
    }

    void Map::deinit_objects()
    {
    }

    void Map::deinit_players()
    {
    }

    void Map::deinit_seasons()
    {
        for (const auto& [name, season] : m_seasons)
        {
            season->deinit();
        }
    }
}
