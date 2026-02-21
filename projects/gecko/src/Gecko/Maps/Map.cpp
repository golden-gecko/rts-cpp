#include "Gecko/Maps/Map.hpp"

#include "Gecko/Configuration.hpp"
#include "Gecko/Exception.hpp"
#include "Gecko/Games/Game.hpp"
#include "Gecko/Layers/SquareLayer.hpp"
#include "Gecko/Log.hpp"
#include "Gecko/Managers/MapManager.hpp"
#include "Gecko/Managers/ObjectManager.hpp"
#include "Gecko/Managers/OrderManager.hpp"
#include "Gecko/Managers/PlayerManager.hpp"
#include "Gecko/Objects/Object.hpp"
#include "Gecko/Players/Player.hpp"
#include "Gecko/Scenes/Scene.hpp"
#include "Gecko/Season.hpp"

namespace Gecko
{
    MapPtr Map::create(MapPtr memory, const ConfigurationPtr& configuration, const ScenePtr& scene)
    {
        auto map = new (memory) Map(scene);

        map->deserialize(configuration);

        return map;
    }

    Map::Map(const ScenePtr& scene) :
        m_scene(scene)
    {
    }

    Map::Map(const Map& other) :
        base_type(other),
        m_scene(other.m_scene)
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

        init_layers();
        init_seasons();

        init_players();
        init_objects();

        set_visible(true);
    }

    void Map::deinit()
    {
        deinit_layers();
        deinit_seasons();

        m_layers.clear();
        m_seasons.clear();

        PlayerManager::getSingleton().destroy_all();
        OrderManager::getSingleton().destroy_all();

        set_visible(false);

        base_type::deinit();
    }

    ConfigurationPtr Map::serialize() const
    {
        auto configuration = base_type::serialize();

        configuration->set("name", m_name);

        return configuration;
    }

    void Map::deserialize(const ConfigurationPtr& configuration)
    {
        base_type::deserialize(configuration);

        m_name = get_configuration()->get_string("name");
    }

    void Map::update(float time)
    {
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
        return get_layer(Settings::Layer::Terrain)->is_position_valid(position);
    }

    void Map::show_data_layer(const std::string& layer_name, const std::string& data_layer_name)
    {
        auto layer = get_layer(layer_name);

        if (layer)
        {
            layer->show_data_layer(data_layer_name);
        }
    }

    CameraPtr Map::get_camera(const std::string& name)
    {
        return m_scene->get_camera(name);
    }

    std::shared_ptr<Layer> Map::get_layer(const std::string& name) const
    {
        auto layer = m_layers.find(name);

        if (layer == m_layers.end())
        {
            L_WARNING << "Map '" << get_name() << "' has no '" << name << "' layer.";

            return nullptr;
        }

        return layer->second;
    }

    std::shared_ptr<Season> Map::get_season(const std::string& name) const
    {
        auto season = m_seasons.find(name);

        if (season == m_seasons.end())
        {
            L_WARNING << "Map '" << get_name() << "' has no '" << name << "' season.";

            return nullptr;
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

    void Map::init_layers()
    {
        m_layers.clear();

        auto layers_configuration = m_configuration->get_child_optional("layers");

        if (layers_configuration)
        {
            for (auto i = layers_configuration.value()->begin(); i != layers_configuration.value()->end(); i++)
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
            for (const auto& i : *(*(players_configuration)))
            {
                auto player_configuration = Configuration(i);
                auto configuration = player_configuration.get_string("configuration");

                for (const auto& [_, player] : PlayerManager::getSingleton())
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
            }
        }
    }

    void Map::init_seasons()
    {
        m_seasons.clear();

        auto seasons_configuration = m_configuration->get_child_optional("seasons");

        if (seasons_configuration)
        {
            for (auto i = seasons_configuration.value()->begin(); i != seasons_configuration.value()->end(); i++)
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

    void Map::deinit_layers()
    {
        for (const auto& [name, layer] : m_layers)
        {
            layer->deinit();
        }
    }

    void Map::deinit_objects()
    {
        auto deinit = [](Item& item)
        {
            item.deinit();
        };

        ObjectManager::getSingleton().iterate(std::bind(deinit, std::placeholders::_1));
        ObjectManager::getSingleton().destroy_all();
    }

    void Map::deinit_players()
    {
        auto deinit = [](Item& item)
        {
            item.deinit();
        };

        PlayerManager::getSingleton().iterate(std::bind(deinit, std::placeholders::_1));
        PlayerManager::getSingleton().destroy_all();
    }

    void Map::deinit_seasons()
    {
        for (const auto& [name, season] : m_seasons)
        {
            season->deinit();
        }
    }
}
