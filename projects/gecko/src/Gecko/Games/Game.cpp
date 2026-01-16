#include "Gecko/Games/Game.hpp"

#include "Gecko/Components/Armour.hpp"
#include "Gecko/Components/Debug.hpp"
#include "Gecko/Components/Drive.hpp"
#include "Gecko/Components/Hull.hpp"
#include "Gecko/Components/Radar.hpp"
#include "Gecko/Components/Shield.hpp"
#include "Gecko/Components/Storage.hpp"
#include "Gecko/Components/Weapon.hpp"
#include "Gecko/Configuration.hpp"
#include "Gecko/Containers/Configurations.hpp"
#include "Gecko/Containers/Selected.hpp"
#include "Gecko/Exception.hpp"
#include "Gecko/Input.hpp"
#include "Gecko/Managers/ComponentManager.hpp"
#include "Gecko/Managers/ConfigurationManager.hpp"
#include "Gecko/Managers/MapManager.hpp"
#include "Gecko/Managers/ObjectManager.hpp"
#include "Gecko/Managers/OrderManager.hpp"
#include "Gecko/Managers/PlayerManager.hpp"
#include "Gecko/Managers/SkillManager.hpp"
#include "Gecko/Maps/Map.hpp"
#include "Gecko/Orders/OrderAttack.hpp"
#include "Gecko/Orders/OrderCreate.hpp"
#include "Gecko/Orders/OrderDestroy.hpp"
#include "Gecko/Orders/OrderFollow.hpp"
#include "Gecko/Orders/OrderGuard.hpp"
#include "Gecko/Orders/OrderLoad.hpp"
#include "Gecko/Orders/OrderMove.hpp"
#include "Gecko/Orders/OrderPatrol.hpp"
#include "Gecko/Orders/OrderRally.hpp"
#include "Gecko/Orders/OrderStop.hpp"
#include "Gecko/Orders/OrderUnload.hpp"
#include "Gecko/Orders/OrderWait.hpp"
#include "Gecko/Objects/Factory.hpp"
#include "Gecko/Objects/Missile.hpp"
#include "Gecko/Objects/Vehicle.hpp"
#include "Gecko/Players/Player.hpp"
#include "Gecko/Skills/Buff.hpp"
#include "Gecko/Skills/FireMissile.hpp"
#include "Gecko/Skills/Repair.hpp"
#include "Gecko/UI/UI.hpp"
#include "Gecko/UI/Cursor.hpp"
#include "Gecko/UI/Minimap.hpp"
#include "Gecko/UI/Preview.hpp"
#include "Gecko/Utils/Convert.hpp"
#include "Gecko/Utils/Mesh.hpp"
#include "Gecko/Utils/Time.hpp"

Gecko::Game* Ogre::Singleton<Gecko::Game>::msSingleton = nullptr;

namespace Gecko
{
    void Game::renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& cameraName, bool& skipThisInvocation)
    {
        UI::getSingleton().render(queueGroupId, cameraName, skipThisInvocation);
    }

    void Game::windowResized(Ogre::RenderWindow* rw)
    {
        OgreBites::ApplicationContext::windowResized(rw);

        unsigned int width, height;
        int top, left;

        rw->getMetrics(width, height, left, top);

        for (unsigned short i = 0; i < rw->getNumViewports(); i++)
        {
            rw->getViewport(i)->getCamera()->setAspectRatio(
                static_cast<Ogre::Real>(rw->getViewport(i)->getActualWidth()) / static_cast<Ogre::Real>(rw->getViewport(i)->getActualHeight())
            );
        }

        Input::getSingleton().set_window_size(static_cast<int>(width), static_cast<int>(height));
    }

    void Game::init()
    {
        init_root();
        init_scene();
        init_meshes();

        init_skills(m_configuration->get_int<std::size_t>("memory.skills", 0));
        init_components(m_configuration->get_int<std::size_t>("memory.components", 0));
        init_orders(m_configuration->get_int<std::size_t>("memory.orders", 0));
        init_players(m_configuration->get_int<std::size_t>("memory.players", 0));
        init_objects(m_configuration->get_int<std::size_t>("memory.objects", 0));
        init_maps(m_configuration->get_int<std::size_t>("memory.maps", 0));
    }

    void Game::deinit()
    {
        MapManager::getSingleton().destroy_all();
        ObjectManager::getSingleton().destroy_all();
        PlayerManager::getSingleton().destroy_all();
        OrderManager::getSingleton().destroy_all();
        ComponentManager::getSingleton().destroy_all();
        SkillManager::getSingleton().destroy_all();

        deinit_scene();
        deinit_root();
    }

    void Game::update(float time)
    {
        // TODO: Optimize to reach 60 frames.

        MapManager::getSingleton().update(time);
        ObjectManager::getSingleton().update(time);
        OrderManager::getSingleton().update(time);
        PlayerManager::getSingleton().update(time);
        UI::getSingleton().update(time);
    }

    Game::Game(const ConfigurationPtr& configuration) :
        m_configuration(configuration)
    {
    }

    void Game::load_map(const std::string& map_name)
    {
        L_TIME("Game::load_map(" + map_name + ")");

        unload_map();

        auto map = MapManager::getSingleton().create(map_name);

        if (map == nullptr)
        {
            throw Exception("Failed to create map '" + map_name + "'.");
        }

        map->init();

        // TODO: Move to function (maybe in UI)?
        // Reset UI.
        std::set<std::string> configurations;

        for (const auto& [name, configuration] : ConfigurationManager::getSingleton())
        {
            if (configuration->get_bool("creatable.by_player", false))
            {
                configurations.emplace(name);
            }
        }

        UI::getSingleton().set_configurations(configurations);
    }

    void Game::load_save(const std::string& save_name)
    {
        quit();

        // TODO: Implement.
    }

    void Game::quit()
    {
        getRoot()->queueEndRendering();
    }

    void Game::run()
    {
        // HACK: Must be called once before the main loop.
        windowResized(getRenderWindow());

        Gecko::Utils::Time::Value previous_time = Utils::Time::get();
        Gecko::Utils::Time::Value current_time;

        float elasped_time = 0.0f;
        float frame_time;

        while (getRoot()->endRenderingQueued() == false)
        {
            current_time = Utils::Time::get();

            frame_time = Utils::Time::get_duration(previous_time, current_time);

            elasped_time += frame_time;

            if (elasped_time >= Settings::Game::FrameTime)
            {
                pollEvents();

                update_input(Settings::Game::FrameTime);
                update(Settings::Game::FrameTime);

                getRoot()->renderOneFrame();

                elasped_time -= std::floor((elasped_time / Settings::Game::FrameTime)) * Settings::Game::FrameTime;
            }

            previous_time = current_time;
        }
    }

    void Game::save()
    {
        /*
        TODO: Fix.
        Json::Value json_root;

        // Save map.
        json_root["maps"].append();

        // Save players.
        Json::Value json_players;

        for (const auto& [_, player] : PlayerManager::getSingleton())
        {
        json_players.append(player->serialize().get());
        }

        json_root["players"] = json_players;

        // Save layers.
        Json::Value json_layers;

        const auto& layers = ;

        for (const auto& [name, layer] : layers)
        {
        Json::Value json_layer;

        const auto& scale = layer->get_scale();
        const auto& heightmap = layer->get_heightmap();
        const auto& material_name = layer->get_material_name();

        if (heightmap.empty() == empty)
        {
        json_layer["heightmap"] = heightmap;
        }

        if (material_name.empty() == empty())
        {
        json_layer["material"] = material_name;
        }

        json_layer["scale"]["x"] = scale.x;
        json_layer["scale"]["y"] = scale.y;
        json_layer["scale"]["z"] = scale.z;

        // Save data layers.
        const auto& data_layers = layer->get_data_layers();

        Json::Value json_data_layers;

        for (const auto& [name, data_layer] : data_layers)
        {
        Json::Value json_data_layer;

        const auto& source = data_layer->get_source();

        if (material_name.empty() == empty())
        {
        json_data_layer["source"] = source;
        }

        json_data_layers[name] = json_data_layer;
        }

        json_layer["data_layers"] = json_data_layers;

        json_layers[name] = json_layer;
        }

        json_root["layers"] = json_layers;

        // Save objects.
        Json::Value json_objects;

        for (const auto& [_, object] : ObjectManager::getSingleton())
        {
        const auto& player_id = object->get_player_id();
        auto player = PlayerManager::getSingleton().get(player_id);

        if (player == nullptr)
        {
        L_WARNING << "Player with ID " << player_id << " not found. Skipping object.";

        continue;
        }

        json_objects.append(object->serialize().get());
        }

        json_root["objects"] = json_objects;

        // TODO: Create function json_to_file.
        {
        auto saves_path = configuration->get_string("options.saves.path", Settings::Game::SavesPath);

        if (exists(saves_path) == false)
        {
        create_directories(saves_path);
        }

        auto save_filename = path(saves_path) / path(Utils::get_filename_from_date() + ".json");

        Utils::json_to_file(json_root, save_filename.string());
        }
        */
    }

    void Game::shutdown()
    {
        getRoot()->queueEndRendering();
    }

    void Game::unload_map()
    {
        L_TIME("Game::unload_map()");

        auto deinit = [](Item& item)
        {
            item.deinit();
        };

        MapManager::getSingleton().iterate(std::bind(deinit, std::placeholders::_1));
        MapManager::getSingleton().destroy_all();

        ComponentManager::getSingleton().iterate(std::bind(deinit, std::placeholders::_1));
        OrderManager::getSingleton().iterate(std::bind(deinit, std::placeholders::_1));

        ComponentManager::getSingleton().destroy_all();
        OrderManager::getSingleton().destroy_all();
    }

    void Game::update_input(float time)
    {
        Input::getSingleton().update(time);
    }

    Ogre::Entity* Game::create_entity(const std::string& name) const
    {
        return m_scene_manager->createEntity(name);
    }

    void Game::destroy_entity(Ogre::Entity* entity) const
    {
        if (entity)
        {
            m_scene_manager->destroyEntity(entity);
        }
    }

    Ogre::ManualObject* Game::create_manual_object() const
    {
        return m_scene_manager->createManualObject();
    }

    void Game::destroy_manual_object(Ogre::ManualObject* manual_object) const
    {
        if (manual_object)
        {
            m_scene_manager->destroyManualObject(manual_object);
        }
    }

    Ogre::RaySceneQuery* Game::create_ray_scene_query(const Ogre::Ray& ray) const
    {
        auto ray_scene_query = m_scene_manager->createRayQuery(ray);

        ray_scene_query->setSortByDistance(true);

        return ray_scene_query;
    }

    void Game::destroy_ray_scene_query(Ogre::RaySceneQuery* ray_scene_query)
    {
        if (ray_scene_query)
        {
            m_scene_manager->destroyQuery(ray_scene_query);
        }
    }

    Ogre::SceneNode* Game::create_scene_node() const
    {
        return m_scene_manager->getRootSceneNode()->createChildSceneNode();
    }

    void Game::destroy_scene_node(Ogre::SceneNode* scene_node) const
    {
        if (scene_node)
        {
            m_scene_manager->destroySceneNode(scene_node);
        }
    }

    void Game::save_options(const std::string& _options)
    {
        L_TRACE << "Game::save_options(" << _options << ")";

        /*
        TODO: Fix.

        std::vector<std::string> options;
        boost::algorithm::split(options, _options, boost::is_any_of("&"), boost::token_compress_on);

        for (const auto& option : options)
        {
            L_DEBUG << "option: " << option;

            std::vector<std::string> key_value;
            boost::algorithm::split(key_value, option, boost::is_any_of("="), boost::token_compress_on);

            if (key_value.size() != 2)
            {
                L_WARNING << "Option " << option << " is incorrect.";

                continue;
            }

            if (key_value[0] == "game_fog_of_war_type")
            {
                // TODO: Implement.
            }
            else if (key_value[0] == "input_mouse_sensivity")
            {
                // TODO: Implement.
                // Input::getSingleton().set_mouse_sensitivity(visible);
            }
            else if (key_value[0] == "ui_cursor_visible")
            {
                bool visible = (key_value[1] == "visible");

                UI::getSingleton().get_cursor().set_visible(visible);
            }
            else if (key_value[0] == "ui_minimap_visible")
            {
                bool visible = (key_value[1] == "visible");

                // UI::getSingleton().get_minimap().set_visible(visible);
            }
            else if (key_value[0] == "ui_preview_visible")
            {
                bool visible = (key_value[1] == "visible");

                UI::getSingleton().get_preview().set_visible(visible);
            }

            if (key_value[0] == "fog_of_war_type")
            {
                auto terrain_texture = Ogre::MaterialManager::getSingleton().getByName("Terrain")
                    ->getTechnique(0)
                    ->getPass(0)
                    ->getTextureUnitState("Fog");

                auto water_texture = Ogre::MaterialManager::getSingleton().getByName("Water02")
                    ->getTechnique(0)
                    ->getPass(0)
                    ->getTextureUnitState("Fog");

                if (key_value[1] == "fuzzy")
                {
                    terrain_texture->setTextureFiltering(Ogre::TextureFilterOptions::TFO_BILINEAR);
                    water_texture->setTextureFiltering(Ogre::TextureFilterOptions::TFO_BILINEAR);
                }
                else if (key_value[1] == "solid")
                {
                    terrain_texture->setTextureFiltering(Ogre::TextureFilterOptions::TFO_NONE);
                    water_texture->setTextureFiltering(Ogre::TextureFilterOptions::TFO_NONE);
                }
            }
            else if (key_value[0] == "sky")
            {
                if (key_value[1] == "enabled")
                {
                    // TODO: Fix.
                    // get_m_scene_manager()->getSkyBoxNode()->setVisible(true);
                }
                else if (key_value[1] == "disabled")
                {
                    // TODO: Fix.
                    // get_m_scene_manager()->getSkyBoxNode()->setVisible(false);
                }
            }
        }
        */
    }

    void Game::load_options()
    {
        ConfigurationPtr options = m_configuration->get_child("options");

        /*
        TODO: Implement.

        Game::getSingleton().set_fog_of_war_color(
            options.get<std::string>("scene.fog_of_war.color.r"),
            options.get<std::string>("scene.fog_of_war.color.g"),
            options.get<std::string>("scene.fog_of_war.color.b"),
            options.get<std::string>("scene.fog_of_war.color.a")
        );

        Game::getSingleton().set_fog_of_war_type(
            options.get<std::string>("fog_of_war.type")
        );
        */

        Input::getSingleton().set_mouse_sensitivity(
            options->get_vector2("input.mouse.sensitivity")
        );

        UI::getSingleton().get_cursor().set_visible(
            options->get_bool("ui.cursor.visible", Settings::UI::CursorVisibility)
        );

        /*
        TODO: Implement.

        UI::getSingleton().get_minimap().set_visible(
            options->get_bool("ui.minimap.visible", Settings::UI::MinimapVisibility)
        );
        */

        UI::getSingleton().get_preview().set_visible(
            options->get_bool("ui.preview.visible", Settings::UI::PreviewVisibility)
        );

        /*
        TODO: Implement.

        const auto fog_of_war_type = options->get_string("scene.map.fog_of_war.type", "fuzzy");

        auto terrain_texture = Ogre::MaterialManager::getSingleton().getByName("Terrain_RTS")
            ->getTechnique(0)
            ->getPass(0)
            ->getTextureUnitState("Fog");

        auto water_texture = Ogre::MaterialManager::getSingleton().getByName("Water02")
            ->getTechnique(0)
            ->getPass(0)
            ->getTextureUnitState("Fog");

        if (fog_of_war_type == "fuzzy")
        {
            terrain_texture->setTextureFiltering(Ogre::TextureFilterOptions::TFO_BILINEAR);
            water_texture->setTextureFiltering(Ogre::TextureFilterOptions::TFO_BILINEAR);
        }
        else if (fog_of_war_type == "solid")
        {
            terrain_texture->setTextureFiltering(Ogre::TextureFilterOptions::TFO_NONE);
            water_texture->setTextureFiltering(Ogre::TextureFilterOptions::TFO_NONE);
        }
        */
    }

    Player* Game::get_active_player() const
    {
        return PlayerManager::getSingleton().get(m_active_player_id);
    }

    std::vector<std::string> Game::get_maps() const
    {
        auto maps_path = m_configuration->get_string("options.maps.path", Settings::Game::SavesPath);

        if (std::filesystem::exists(maps_path) == false)
        {
            return {};
        }

        std::vector<std::string> maps;

        for (auto directory = std::filesystem::directory_iterator(maps_path); directory != std::filesystem::directory_iterator(); ++directory)
        {
            if (std::filesystem::is_directory(directory->path()))
            {
                for (auto file = std::filesystem::directory_iterator(directory->path()); file != std::filesystem::directory_iterator(); ++file)
                {
                    if (file->path().extension() == Settings::Configuration::Extension)
                    {
                        maps.emplace_back(file->path().stem().string());
                    }
                }
            }
        }

        return maps;
    }

    std::vector<std::string> Game::get_saves() const
    {
        auto saves_path = m_configuration->get_string("options.saves.path", Settings::Game::SavesPath);

        if (std::filesystem::exists(saves_path) == false)
        {
            return {};
        }

        std::vector<std::string> saves;

        for (auto file = std::filesystem::directory_iterator(saves_path); file != std::filesystem::directory_iterator(); ++file)
        {
            if (file->path().extension() == Settings::Configuration::Extension)
            {
                saves.emplace_back(file->path().stem().string());
            }
        }

        return saves;
    }

    void Game::set_active_player_id(const Id& id)
    {
        UI::getSingleton().log_info(
            "Switched active player from "
            + Utils::Convert::to_string(m_active_player_id)
            + " to "
            + Utils::Convert::to_string(id)
            + "."
        );

        // Get current active player.
        auto current_active_player = PlayerManager::getSingleton().get(m_active_player_id);

        if (current_active_player)
        {
            current_active_player->get_selected()->apply_current_selection(false);
            current_active_player->get_selected()->apply_ui(false);
        }

        // Set new active player ID.
        m_active_player_id = id;

        // Get new active player.
        auto new_active_player = PlayerManager::getSingleton().get(id);

        if (new_active_player)
        {
            new_active_player->get_selected()->apply_current_selection(true);
            new_active_player->get_selected()->apply_ui(true);

            // TODO: Fix.
            // set_fog_of_war_texture(new_active_player->get_fog_of_war_texture());
            // new_active_player->apply_fog_of_war();
        }
    }

    void Game::init_components(std::size_t max_size)
    {
        L_TIME("Gecko::Game::init_components()");

        auto& component_manager = ComponentManager::getSingleton();

        for (const auto& [name, configuration] : ConfigurationManager::getSingleton())
        {
            L_INFO << "Loading '" << name << "' configuration.";

            // TODO: Remove default value.
            auto type = configuration->get_string("type", "");

            if (type == "Armour")
            {
                auto factory = std::bind(Armour::create, std::placeholders::_1, configuration);

                component_manager.register_type<Armour>(name, factory);
                component_manager.allocate(name, max_size);
            }
            else if (type == "Debug")
            {
                auto factory = std::bind(Debug::create, std::placeholders::_1, configuration);

                component_manager.register_type<Debug>(name, factory);
                component_manager.allocate(name, max_size);
            }
            else if (type == "Drive")
            {
                auto factory = std::bind(Drive::create, std::placeholders::_1, configuration);

                component_manager.register_type<Drive>(name, factory);
                component_manager.allocate(name, max_size);
            }
            else if (type == "Hull")
            {
                auto factory = std::bind(Hull::create, std::placeholders::_1, configuration);

                component_manager.register_type<Hull>(name, factory);
                component_manager.allocate(name, max_size);
            }
            else if (type == "Radar")
            {
                auto factory = std::bind(Radar::create, std::placeholders::_1, configuration);

                component_manager.register_type<Radar>(name, factory);
                component_manager.allocate(name, max_size);
            }
            else if (type == "Shield")
            {
                auto factory = std::bind(Shield::create, std::placeholders::_1, configuration);

                component_manager.register_type<Shield>(name, factory);
                component_manager.allocate(name, max_size);
            }
            else if (type == "Storage")
            {
                auto factory = std::bind(Storage::create, std::placeholders::_1, configuration);

                component_manager.register_type<Storage>(name, factory);
                component_manager.allocate(name, max_size);
            }
            else if (type == "Weapon")
            {
                auto factory = std::bind(Weapon::create, std::placeholders::_1, configuration);

                component_manager.register_type<Weapon>(name, factory);
                component_manager.allocate(name, max_size);
            }
        }
    }

    void Game::init_maps(std::size_t max_size)
    {
        L_TIME("Gecko::Game::init_maps()");

        auto& map_manager = MapManager::getSingleton();

        for (const auto& [name, configuration] : ConfigurationManager::getSingleton())
        {
            L_INFO << "Loading '" << name << "' configuration.";

            // TODO: Remove default value.
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

    void Game::init_objects(std::size_t max_size)
    {
        L_TIME("Gecko::Game::init_objects()");

        auto& object_manager = ObjectManager::getSingleton();

        for (const auto& [name, configuration] : ConfigurationManager::getSingleton())
        {
            L_INFO << "Loading '" << name << "' configuration.";

            // TODO: Remove default value.
            auto type = configuration->get_string("type", "");

            if (type == "Factory")
            {
                auto factory = std::bind(Factory::create, std::placeholders::_1, configuration);

                object_manager.register_type<Factory>(name, factory);
                object_manager.allocate(name, max_size);
            }
            else if (type == "Missile")
            {
                auto factory = std::bind(Missile::create, std::placeholders::_1, configuration);

                object_manager.register_type<Missile>(name, factory);
                object_manager.allocate(name, max_size);
            }
            else if (type == "Object")
            {
                auto factory = std::bind(Object::create, std::placeholders::_1, configuration);

                object_manager.register_type<Object>(name, factory);
                object_manager.allocate(name, max_size);
            }
            else if (type == "Vehicle")
            {
                auto factory = std::bind(Vehicle::create, std::placeholders::_1, configuration);

                object_manager.register_type<Vehicle>(name, factory);
                object_manager.allocate(name, max_size);
            }
        }
    }

    void Game::init_orders(std::size_t max_size)
    {
        L_TIME("Gecko::Game::init_orders()");

        auto& order_manager = OrderManager::getSingleton();

        for (const auto& [name, configuration] : ConfigurationManager::getSingleton())
        {
            L_INFO << "Loading '" << name << "' configuration.";

            // TODO: Remove default value.
            auto type = configuration->get_string("type", "");

            if (type == "Attack")
            {
                auto function = static_cast<OrderAttack*(*)(OrderAttack* memory, const ConfigurationPtr&)>(&OrderAttack::create);
                auto factory = std::bind(function, std::placeholders::_1, configuration);

                order_manager.register_type<OrderAttack>(name, factory);
                order_manager.allocate(name, max_size);
            }
            else if (type == "Create")
            {
                auto function = static_cast<OrderCreate*(*)(OrderCreate* memory, const ConfigurationPtr&)>(&OrderCreate::create);
                auto factory = std::bind(function, std::placeholders::_1, configuration);

                order_manager.register_type<OrderCreate>(name, factory);
                order_manager.allocate(name, max_size);
            }
            else if (type == "Destroy")
            {
                auto function = static_cast<OrderDestroy*(*)(OrderDestroy* memory, const ConfigurationPtr&)>(&OrderDestroy::create);
                auto factory = std::bind(function, std::placeholders::_1, configuration);

                order_manager.register_type<OrderDestroy>(name, factory);
                order_manager.allocate(name, max_size);
            }
            else if (type == "Follow")
            {
                auto function = static_cast<OrderFollow*(*)(OrderFollow* memory, const ConfigurationPtr&)>(&OrderFollow::create);
                auto factory = std::bind(function, std::placeholders::_1, configuration);

                order_manager.register_type<OrderFollow>(name, factory);
                order_manager.allocate(name, max_size);
            }
            else if (type == "Guard")
            {
                auto function = static_cast<OrderGuard*(*)(OrderGuard* memory, const ConfigurationPtr&)>(&OrderGuard::create);
                auto factory = std::bind(function, std::placeholders::_1, configuration);

                order_manager.register_type<OrderGuard>(name, factory);
                order_manager.allocate(name, max_size);
            }
            else if (type == "Load")
            {
                auto function = static_cast<OrderLoad*(*)(OrderLoad* memory, const ConfigurationPtr&)>(&OrderLoad::create);
                auto factory = std::bind(function, std::placeholders::_1, configuration);

                order_manager.register_type<OrderLoad>(name, factory);
                order_manager.allocate(name, max_size);
            }
            else if (type == "Move")
            {
                auto function = static_cast<OrderMove*(*)(OrderMove* memory, const ConfigurationPtr&)>(&OrderMove::create);
                auto factory = std::bind(function, std::placeholders::_1, configuration);

                order_manager.register_type<OrderMove>(name, factory);
                order_manager.allocate(name, max_size);
            }
            else if (type == "Patrol")
            {
                auto function = static_cast<OrderPatrol*(*)(OrderPatrol* memory, const ConfigurationPtr&)>(&OrderPatrol::create);
                auto factory = std::bind(function, std::placeholders::_1, configuration);

                order_manager.register_type<OrderPatrol>(name, factory);
                order_manager.allocate(name, max_size);
            }
            else if (type == "Rally")
            {
                auto function = static_cast<OrderRally*(*)(OrderRally* memory, const ConfigurationPtr&)>(&OrderRally::create);
                auto factory = std::bind(function, std::placeholders::_1, configuration);

                order_manager.register_type<OrderRally>(name, factory);
                order_manager.allocate(name, max_size);
            }
            else if (type == "Stop")
            {
                auto function = static_cast<OrderStop*(*)(OrderStop* memory, const ConfigurationPtr&)>(&OrderStop::create);
                auto factory = std::bind(function, std::placeholders::_1, configuration);

                order_manager.register_type<OrderStop>(name, factory);
                order_manager.allocate(name, max_size);
            }
            else if (type == "Unload")
            {
                auto function = static_cast<OrderUnload*(*)(OrderUnload* memory, const ConfigurationPtr&)>(&OrderUnload::create);
                auto factory = std::bind(function, std::placeholders::_1, configuration);

                order_manager.register_type<OrderUnload>(name, factory);
                order_manager.allocate(name, max_size);
            }
            else if (type == "Wait")
            {
                auto function = static_cast<OrderWait*(*)(OrderWait* memory, const ConfigurationPtr&)>(&OrderWait::create);
                auto factory = std::bind(function, std::placeholders::_1, configuration);

                order_manager.register_type<OrderWait>(name, factory);
                order_manager.allocate(name, max_size);
            }
        }
    }

    void Game::init_players(std::size_t max_size)
    {
        L_TIME("Gecko::Game::init_players()");

        auto& player_manager = PlayerManager::getSingleton();

        for (const auto& [name, configuration] : ConfigurationManager::getSingleton())
        {
            L_INFO << "Loading '" << name << "' configuration.";

            // TODO: Remove default value.
            auto type = configuration->get_string("type", "");

            if (type == "Player")
            {
                auto function = static_cast<Player*(*)(Player* memory, const ConfigurationPtr&)>(&Player::create);
                auto factory = std::bind(function, std::placeholders::_1, configuration);

                player_manager.register_type<Player>(name, factory);
                player_manager.allocate(name, max_size);
            }
        }
    }

    void Game::init_skills(std::size_t max_size)
    {
        L_TIME("Gecko::Game::init_skills()");

        auto& skill_manager = SkillManager::getSingleton();

        for (const auto& [name, configuration] : ConfigurationManager::getSingleton())
        {
            L_INFO << "Loading '" << name << "' configuration.";

            // TODO: Remove default value.
            auto type = configuration->get_string("type", "");

            if (type == "Buff")
            {
                auto factory = std::bind(Buff::create, std::placeholders::_1, configuration);

                skill_manager.register_type<Buff>(name, factory);
                skill_manager.allocate(name, max_size);
            }
            else if (type == "FireMissile")
            {
                auto factory = std::bind(FireMissile::create, std::placeholders::_1, configuration);

                skill_manager.register_type<FireMissile>(name, factory);
                skill_manager.allocate(name, max_size);
            }
            else if (type == "Repair")
            {
                auto factory = std::bind(Repair::create, std::placeholders::_1, configuration);

                skill_manager.register_type<Repair>(name, factory);
                skill_manager.allocate(name, max_size);
            }
        }
    }

    void Game::init_meshes()
    {
        auto meshes_path = m_configuration->get_string("options.cache.meshes.path", Settings::Cache::MeshesPath);
        const auto& mesh_name = Settings::UI::SelectionMesh;
        auto mesh_path = std::filesystem::path(meshes_path) / std::filesystem::path(mesh_name);

        if (std::filesystem::exists(mesh_path) == false)
        {
            auto selection = create_manual_object();

            // TODO: Hardcoded.
            selection->begin("white");

            selection->position(-0.5f, 0.0f, -0.5f);
            selection->normal(Ogre::Vector3::UNIT_Y);
            selection->textureCoord(0.0f, 0.0f);

            selection->position(-0.5f, 0.0f, 0.5f);
            selection->normal(Ogre::Vector3::UNIT_Y);
            selection->textureCoord(1.0f, 0.0f);

            selection->position(0.5f, 0.0f, 0.5f);
            selection->normal(Ogre::Vector3::UNIT_Y);
            selection->textureCoord(1.0f, 1.0f);

            selection->position(0.5f, 0.0f, -0.5f);
            selection->normal(Ogre::Vector3::UNIT_Y);
            selection->textureCoord(0.0f, 1.0f);

            selection->index(0);
            selection->index(1);
            selection->index(3);

            selection->index(3);
            selection->index(1);
            selection->index(2);

            selection->end();

            // Save mesh to cache.
            auto mesh = selection->convertToMesh(mesh_name);

            if (m_configuration->get_bool("options.cache.meshes.enabled", true))
            {
                Ogre::MeshSerializer serializer;

                if (std::filesystem::exists(meshes_path) == false)
                {
                    std::filesystem::create_directories(meshes_path);
                }

                serializer.exportMesh(mesh.get(), mesh_path.string());
            }

            destroy_manual_object(selection);
        }
    }

    void Game::init_root()
    {
        initApp();

        if (getRoot()->restoreConfig() == false)
        {
            getRoot()->showConfigDialog(OgreBites::getNativeConfigDialog());
        }
    }

    void Game::init_scene()
    {
        // TODO: Use "OctreeSceneManager".
        m_scene_manager = getRoot()->createSceneManager();
        m_scene_manager->addRenderQueueListener(this);
        m_scene_manager->setAmbientLight(m_configuration->get_color("scene.ambient.color", Ogre::ColourValue::White));

        // TODO: Move materials to bin directory.
        // TODO: Move to configuration.
        // TODO: Enable.
        // m_scene_manager->setShadowTechnique(Ogre::ShadowTechnique::SHADOWTYPE_STENCIL_MODULATIVE);
        // m_scene_manager->setFog(Ogre::FogMode::FOG_EXP2, Ogre::ColourValue(0.9f, 0.9f, 0.9f), 0.002f, 100.0f, 500.0f);
        // m_scene_manager->setSkyBox(true, "Examples/CloudyNoonSkyBox", 10.0f);

        Ogre::RTShader::ShaderGenerator::getSingleton().addSceneManager(m_scene_manager);

        m_light = m_scene_manager->createLight();
        // TODO: Makes all material black. Investigate.
        // light->setDiffuseColour(m_configuration->get_color("scene.directional.color", Ogre::ColourValue::White));
        m_light->setType(Ogre::Light::LightTypes::LT_DIRECTIONAL);

        m_light_scene_node = create_scene_node();
        m_light_scene_node->attachObject(m_light);
        // TODO: Makes some parts of objects white. Investigate.
        // light_scene_node->setDirection(m_configuration->get_vector3("scene.directional.direction"));
    }

    void Game::deinit_maps()
    {
        for (const auto& [_, map] : MapManager::getSingleton())
        {
            map->deinit();
        }
    }

    void Game::deinit_root()
    {
        closeApp();
    }

    void Game::deinit_scene()
    {
        Ogre::RTShader::ShaderGenerator::getSingleton().removeSceneManager(m_scene_manager);

        getRoot()->destroySceneManager(m_scene_manager);
    }
}
