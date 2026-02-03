#include "Gecko/Games/Game.hpp"

#include "Gecko/Components/Component.hpp"
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
#include "Gecko/Orders/Order.hpp"
#include "Gecko/Objects/Object.hpp"
#include "Gecko/Players/Player.hpp"
#include "Gecko/Skills/Skill.hpp"
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
        init_scene(m_configuration->get_child("scene"));
        init_meshes();
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
        // Must be updated first.
        Input::getSingleton().update(time);

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

        m_active_map = MapManager::getSingleton().create(map_name);

        if (m_active_map == nullptr)
        {
            throw Exception("Failed to create map '" + map_name + "'.");
        }

        m_active_map->init();
    }

    void Game::load_save(const std::string& save_name)
    {
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

                update(Settings::Game::FrameTime);

                elasped_time -= std::floor((elasped_time / Settings::Game::FrameTime)) * Settings::Game::FrameTime;
            }

            getRoot()->renderOneFrame();

            previous_time = current_time;
        }
    }

    void Game::save()
    {
        /*
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

        {
        auto saves_path = Settings::Game::SavesPath;

        if (exists(saves_path) == false)
        {
        create_directories(saves_path);
        }

        auto save_filename = path(saves_path) / path(Utils::get_filename_from_date() + ".json");

        Utils::json_to_file(json_root, save_filename.string());
        }
        */
    }

    void Game::stop()
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

    Ogre::PlaneBoundedVolumeListSceneQuery* Game::create_plane_volume_query(const Ogre::PlaneBoundedVolumeList& volumes, Ogre::uint32 mask) const
    {
        return m_scene_manager->createPlaneBoundedVolumeQuery(volumes, mask);
    }

    void Game::destroy_query(Ogre::SceneQuery* scene_query)
    {
        if (scene_query)
        {
            m_scene_manager->destroyQuery(scene_query);
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
            }
            else if (key_value[0] == "input_mouse_sensivity")
            {
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
                    // get_m_scene_manager()->getSkyBoxNode()->setVisible(true);
                }
                else if (key_value[1] == "disabled")
                {
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
        UI::getSingleton().get_minimap().set_visible(
            options->get_bool("ui.minimap.visible", Settings::UI::MinimapVisibility)
        );

        UI::getSingleton().get_preview().set_visible(
            options->get_bool("ui.preview.visible", Settings::UI::PreviewVisibility)
        );

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

    MapPtr Game::get_active_map() const
    {
        return MapManager::getSingleton().begin()->second; // TODO: Hardcoded.
    }

    PlayerPtr Game::get_active_player() const
    {
        return PlayerManager::getSingleton().get(m_active_player_id);
    }

    std::vector<std::string> Game::get_maps() const
    {
        if (std::filesystem::exists(Settings::Game::SavesPath) == false)
        {
            return {};
        }

        std::vector<std::string> maps;

        for (auto directory = std::filesystem::directory_iterator(Settings::Game::SavesPath); directory != std::filesystem::directory_iterator(); ++directory)
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
        if (std::filesystem::exists(Settings::Game::SavesPath) == false)
        {
            return {};
        }

        std::vector<std::string> saves;

        for (auto file = std::filesystem::directory_iterator(Settings::Game::SavesPath); file != std::filesystem::directory_iterator(); ++file)
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

    void Game::init_meshes()
    {
        auto meshes_path = Settings::Cache::MeshesPath;
        auto mesh_path = std::filesystem::path(meshes_path) / std::filesystem::path(Settings::UI::SelectionMesh);

        if (std::filesystem::exists(mesh_path) == false)
        {
            auto selection = create_manual_object();

            selection->begin("white"); // TODO: Hardcoded.

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
            auto mesh = selection->convertToMesh(Settings::UI::SelectionMesh);

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

    void Game::init_scene(const ConfigurationPtr& configuration)
    {
        m_scene_manager = getRoot()->createSceneManager();
        m_scene_manager->addRenderQueueListener(this);
        m_scene_manager->setAmbientLight(configuration->get_color("ambient.color", Ogre::ColourValue::White));
        m_scene_manager->setFog(Ogre::FogMode::FOG_EXP2,
            configuration->get_color("fog.color", Ogre::ColourValue::White),
            configuration->get_float("fog.density", 0.0f),
            configuration->get_float("fog.start", 0.0f),
            configuration->get_float("fog.end", 0.0f)
        );
        m_scene_manager->setShadowTechnique(Ogre::ShadowTechnique::SHADOWTYPE_STENCIL_MODULATIVE);
        m_scene_manager->setSkyBox(true, configuration->get_string("sky.name"), configuration->get_float("sky.distance", 0.0f));

        Ogre::RTShader::ShaderGenerator::getSingleton().addSceneManager(m_scene_manager);

        m_light = m_scene_manager->createLight();
        m_light->setDiffuseColour(configuration->get_color("directional.diffuse.color", Ogre::ColourValue::White));
        m_light->setSpecularColour(configuration->get_color("directional.specular.color", Ogre::ColourValue::White));
        m_light->setType(Ogre::Light::LightTypes::LT_DIRECTIONAL);

        m_light_scene_node = create_scene_node();
        m_light_scene_node->attachObject(m_light);
        m_light_scene_node->setDirection(configuration->get_vector3("directional.direction"));
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
