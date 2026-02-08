#include "Gecko/UI/UI.hpp"

#include "Gecko/Cameras/ObjectFollowCamera.hpp"
#include "Gecko/Configuration.hpp"
#include "Gecko/Containers/Components.hpp"
#include "Gecko/Containers/Orders.hpp"
#include "Gecko/Containers/Resources.hpp"
#include "Gecko/Containers/Selected.hpp"
#include "Gecko/Games/Game.hpp"
#include "Gecko/Input.hpp"
#include "Gecko/Layers/Layer.hpp"
#include "Gecko/Log.hpp"
#include "Gecko/Managers/MapManager.hpp"
#include "Gecko/Managers/ObjectManager.hpp"
#include "Gecko/Managers/OrderManager.hpp"
#include "Gecko/Managers/PlayerManager.hpp"
#include "Gecko/Maps/Map.hpp"
#include "Gecko/Objects/Object.hpp"
#include "Gecko/Orders/OrderMove.hpp"
#include "Gecko/Players/Player.hpp"
#include "Gecko/Statistics.hpp"
#include "Gecko/System.hpp"
#include "Gecko/Technologies/TechnologyTree.hpp"
#include "Gecko/UI/EventListener.hpp"
#include "Gecko/UI/EventListenerInstancer.hpp"
#include "Gecko/UI/Indicators/Indicator.hpp"
#include "Gecko/UI/RenderInterface.hpp"
#include "Gecko/UI/SystemInterface.hpp"
#include "Gecko/UI/Widgets/Configurations.hpp"
#include "Gecko/UI/Widgets/Console.hpp"
#include "Gecko/UI/Widgets/Cursor.hpp"
#include "Gecko/UI/Widgets/Diplomacy.hpp"
#include "Gecko/UI/Widgets/Info.hpp"
#include "Gecko/UI/Widgets/Log.hpp"
#include "Gecko/UI/Widgets/Minimap.hpp"
#include "Gecko/UI/Widgets/Orders.hpp"
#include "Gecko/UI/Widgets/Players.hpp"
#include "Gecko/UI/Widgets/Preview.hpp"
#include "Gecko/UI/Widgets/Resources.hpp"
#include "Gecko/UI/Widgets/SelectionBox.hpp"
#include "Gecko/UI/Widgets/Skills.hpp"
#include "Gecko/UI/Widgets/Statistics.hpp"
#include "Gecko/Utils/Utils.hpp"
#include "Gecko/Utils/Convert.hpp"
#include "Gecko/Utils/String.hpp"
#include "Gecko/Utils/Time.hpp"

Gecko::UI* Ogre::Singleton<Gecko::UI>::msSingleton = nullptr;

namespace Gecko
{
    /*
    TODO: Implement.

    class IndicatorManager :
        public Manager<Indicator, std::string, Id>
    {
    };
    */

    void UI::init()
    {
        Ogre::RenderWindow* render_window = Game::getSingleton().getRenderWindow();

        m_render_interface = std::make_shared<RenderInterface>(render_window->getWidth(), render_window->getHeight());
        m_system_interface = std::make_shared<SystemInterface>();

        Rml::SetRenderInterface(m_render_interface.get());
        Rml::SetSystemInterface(m_system_interface.get());

        Rml::Initialise();

        m_context = Rml::CreateContext("main", Rml::Vector2i(render_window->getWidth(), render_window->getHeight()));
        m_event_listener_instancer = std::make_shared<EventListenerInstancer>();

    	Rml::Factory::RegisterEventListenerInstancer(m_event_listener_instancer.get());

        init_components();
        init_data_bindings();
        init_fonts();
        init_documents();
        init_events();
        init_visibility_types();
    }

    void UI::deinit()
    {
        /*
        m_console->deinit_events(m_document->GetElementById("console"));
        m_minimap->deinit_events(m_document->GetElementById("minimap"));
        m_preview->deinit_events(m_document->GetElementById("preview"));

        m_console.reset();
        m_cursor.reset();
        m_minimap.reset();
        m_preview.reset();
        m_selection_box.reset();

        refresh_indicators(Id::Empty);
        */

        m_context->UnloadAllDocuments();

        Rml::Shutdown();
    }

    void UI::update(float time)
    {
        if (m_refresh_time.update(time))
        {
            m_refresh_time.reset();

            ObjectPtr hovered_object = ObjectManager::getSingleton().get(m_hovered_object_id);

            if (hovered_object)
            {
                set_info(hovered_object->get_info());

                refresh_indicators(m_hovered_object_id);

                m_preview->get_camera()->set_target_id(hovered_object->get_id());
            }
            else if (Game::getSingleton().get_active_player() && Game::getSingleton().get_active_player()->get_selected()->size())
            {
                Id selected_id = Game::getSingleton().get_active_player()->get_first_selected();
                ObjectPtr selected = ObjectManager::getSingleton().get(selected_id);

                if (selected)
                {
                    set_info(selected->get_info());

                    refresh_indicators(selected_id);

                    m_preview->get_camera()->set_target_id(selected_id);
                }
            }
            else
            {
                System::memory_t memory = System::get_memory_usage();
                const Ogre::RenderTarget::FrameStats& window_statistics = Game::getSingleton().getRenderWindow()->getStatistics();

                Json::Value info;

                info["Average FPS"] = Utils::Convert::to_string(window_statistics.avgFPS, 2);
                info["Last FPS"] = Utils::Convert::to_string(window_statistics.lastFPS, 2);
                info["Triangles"] = window_statistics.triangleCount;
                info["Active player ID"] = Game::getSingleton().get_active_player_id();
                info["Cursor"] = m_cursor->get_position().to_string();
                info["Maps"] = MapManager::getSingleton().size();
                info["Name"] = Game::getSingleton().get_name();
                info["Objects"] = ObjectManager::getSingleton().size();
                info["Orders"] = OrderManager::getSingleton().size();
                info["Players"] = PlayerManager::getSingleton().size();

                if (Game::getSingleton().get_active_player())
                {
                    info["Active player name"] = Game::getSingleton().get_active_player()->get_name();
                    info["Selected"] = Game::getSingleton().get_active_player()->get_selected()->size();
                }

                info["Frame number"] = Utils::Convert::to_string(Game::getSingleton().get_frame_number());
                info["CPU"] = Utils::Convert::to_string(System::get_cpu_usage()) + "%";
                info["Memory"] = Utils::Convert::to_string(static_cast<float>(memory.virtual_memory) / 1024.0f / 1024.0f) + " MB";

                set_info(std::make_shared<Configuration>(info));

                /*
                TODO: Enable.
                const auto& seasons = MapManager::getSingleton().get_items().begin()->second->get_seasons();

                for (const auto& i : seasons)
                {
                    info.emplace(i.get_name(), i.get_current().get_name());
                }

                refresh_indicators(Id::Empty);
                */
            }

            m_players->update();
            m_statistics->update();

            // TODO: Optimize.
            // set_floating_descriptions();

            // TODO: Enable.
            // set_diplomacy();
            // set_layers(Game::getSingleton()->get_active_map()->get_layers());
            // set_maps(Game::getSingleton().get_maps());
            // set_objects_admin();
            // set_orders_admin();

            if (Game::getSingleton().get_active_player())
            {
                m_resources->update(Game::getSingleton().get_active_player()->get_resources());
            }

            // TODO: Enable.
            // set_saves(Game::getSingleton().get_saves());
        }
    }

    UI::UI(const ConfigurationPtr& configuration) :
        m_configuration(configuration)
    {
    }

    void UI::render(Ogre::uint8 queueGroupId, const Ogre::String& cameraName, bool& skipThisInvocation)
    {
        // L_TIME("UI::render()");

        if (queueGroupId != Ogre::RENDER_QUEUE_OVERLAY)
        {
            return;
        }

        if (Ogre::Root::getSingleton().getRenderSystem()->_getViewport()->getOverlaysEnabled() == false)
        {
            return;
        }

        Ogre::RenderSystem* render_system = Game::getSingleton().getRoot()->getRenderSystem();

        if (render_system == nullptr)
        {
            return;
        }

        auto render_window = Game::getSingleton().getRenderWindow();

        if (render_window == nullptr)
        {
            return;
        }

        // Set up the projection and view matrices.
        float z_near = -1.0f;
        float z_far = 1.0f;

        Ogre::Matrix4 projection_matrix = Ogre::Matrix4::ZERO;

        projection_matrix[0][0] = 2.0f / (Ogre::Real)render_window->getWidth();
        projection_matrix[0][3] = -1.0000000f;
        projection_matrix[1][1] = -2.0f / (Ogre::Real)render_window->getHeight();
        projection_matrix[1][3] = 1.0000000f;
        projection_matrix[2][2] = -2.0f / (z_far - z_near);
        projection_matrix[3][3] = 1.0000000f;
        
        render_system->_setProjectionMatrix(projection_matrix);
        render_system->_setViewMatrix(Ogre::Matrix4::IDENTITY);
        render_system->setLightingEnabled(false);
        render_system->_setDepthBufferParams(false, false);
        render_system->_setCullingMode(Ogre::CULL_CLOCKWISE);
        render_system->_setFog(Ogre::FOG_NONE);
        render_system->_setColourBufferWriteEnabled(true, true, true, true);
        render_system->unbindGpuProgram(Ogre::GPT_FRAGMENT_PROGRAM);
        render_system->unbindGpuProgram(Ogre::GPT_VERTEX_PROGRAM);
        render_system->_setTextureCoordSet(0, 0);
        render_system->_setTextureCoordCalculation(0, Ogre::TEXCALC_NONE);
        render_system->_setTextureMatrix(0, Ogre::Matrix4::IDENTITY);
        render_system->_setAlphaRejectSettings(Ogre::CMPF_GREATER, 0, false);
        render_system->_disableTextureUnitsFrom(1);
        render_system->_setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);
        render_system->_setDepthBias(0, 0);

        m_context->Update();
        m_context->Render();
    }

    void UI::change_visibility(const std::string& type, bool visible)
    {
        /*
        auto objects = ObjectManager::getSingleton().get_by_configuration_name(type);

        for (const auto& [id, object] : objects)
        {
        object->set_visible(visible);
        }
        */
    }

    /*
    void UI::engine_application_save_options(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args)
    {
        if (args.size() == 1 && args[0].IsString())
        {
            Game::getSingleton().save_options(Utils::Convert::to_string(args[0]));
        }
    }

    void UI::engine_application_quit(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args)
    {
        Game::getSingleton().shutdown();
    }

    void UI::engine_game_new(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args)
    {
        if (args.size() == 1 && args[0].IsString())
        {
            Game::getSingleton().load_map(Utils::Convert::to_string(args[0]));
        }
    }

    void UI::engine_game_load(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args)
    {
        if (args.size() == 1 && args[0].IsNumber() && args[1].IsString())
        {
            Game::getSingleton().load_save(Utils::Convert::to_string(args[0]));
        }
    }

    void UI::engine_game_save(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args)
    {
        Game::getSingleton().save();
    }

    void UI::engine_game_quit(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args)
    {
        Game::getSingleton().quit();
    }

    void UI::engine_map_set_data_layer(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args)
    {
        if (args.size() == 2 && args[0].IsString() && args[1].IsString())
        {
            // TODO: First map.
            MapManager::getSingleton().begin()->second->show_data_layer(
                Utils::Convert::to_string(args[0]), Utils::Convert::to_string(args[1])
            );
        }
    }

    void UI::engine_minimap_click(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args)
    {
        if (args.size() == 2 && args[0].IsNumber() && args[1].IsNumber())
        {
            get_minimap().click(args[0].ToInteger(), args[1].ToInteger());
        }
    }

    void UI::engine_minimap_move(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args)
    {
        if (args.size() == 2 && args[0].IsNumber() && args[1].IsNumber())
        {
            get_minimap().move(args[0].ToInteger(), args[1].ToInteger());
        }
    }

    void UI::engine_minimap_zoom_in(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args)
    {
        get_minimap().zoom_in();
    }

    void UI::engine_minimap_zoom_out(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args)
    {
        get_minimap().zoom_out();
    }

    void UI::engine_preview_hide(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args)
    {
        get_preview().set_visible(false);
    }

    void UI::engine_preview_set_position(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args)
    {
        if (args.size() == 3 && args[0].IsNumber() && args[1].IsNumber() && args[2].IsNumber())
        {
            get_preview().set_position(args[0].ToInteger(), args[1].ToInteger(), args[2].ToInteger());
        }
    }

    void UI::engine_preview_show(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args)
    {
        get_preview().set_visible(true);
    }

    void UI::engine_technology_research(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args)
    {
        if (args.size() == 1 && args[0].IsString())
        {
            TechnologyTree::getSingleton().research(Utils::Convert::to_string(args[0]));
        }
    }

    void UI::engine_ui_change_visibility(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args)
    {
        if (args.size() == 2 && args[0].IsString() && args[1].IsBoolean())
        {
            change_visibility(Utils::Convert::to_string(args[0]), args[1].ToBoolean());
        }
    }

    void UI::engine_ui_look_at_object(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args)
    {
        if (args.size() == 1 && args[0].IsNumber())
        {
            auto object_id = args[0].ToInteger();
            auto object = ObjectManager::getSingleton().get(object_id);

            if (object)
            {
                auto map = MapManager::getSingleton().begin()->second;

                Game::getSingleton().get_active_player()->get_selected()->select(object_id);

                auto camera = map->get_camera(Settings::Camera::MainName)->get_camera_node();

                camera->setPosition(object->get_position() + Settings::UI::LookAtObjectOffset);
                camera->lookAt(object->get_position(), Ogre::Node::TransformSpace::TS_PARENT);
            }
        }
    }

    void UI::engine_ui_select_object(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args)
    {
        if (args.size() == 1 && args[0].IsNumber())
        {
            Game::getSingleton().get_active_player()->get_selected()->select(args[0].ToNumber());
        }
    }

    void UI::engine_ui_set_configuration(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args)
    {
        if (args.size() == 1 && args[0].IsString())
        {
            set_configuration_name(Utils::Convert::to_string(args[0]));
        }
    }

    void UI::engine_ui_set_order(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args)
    {
        static auto destroy_order = []()
        {
            for (const auto& object_id : *(Game::getSingleton().get_active_player()->get_selected()))
            {
                auto object = ObjectManager::getSingleton().get(object_id);

                if (object)
                {
                    auto order = OrderManager::getSingleton().order_destroy(Id::Empty, object_id);

                    if (Input::getSingleton().is_key_pressed(Command::Value::Multiple_Order) == false)
                    {
                        object->get_orders()->remove_all_orders();
                    }

                    object->get_orders()->add_last(order->get_id());
                }
            }
        };

        static auto disable_order = []()
        {
            for (const auto& object_id : *(Game::getSingleton().get_active_player()->get_selected()))
            {
                auto object = ObjectManager::getSingleton().get(object_id);

                if (object)
                {
                    if (Input::getSingleton().is_key_pressed(Command::Value::Multiple_Order) == false)
                    {
                        object->get_orders()->remove_all_orders();
                    }

                    auto order = OrderManager::getSingleton().order_disable(Id::Empty, object_id);

                    object->get_orders()->add_last(order->get_id());
                }
            }
        };

        static auto enable_order = []()
        {
            for (const auto& object_id : *(Game::getSingleton().get_active_player()->get_selected()))
            {
                auto object = ObjectManager::getSingleton().get(object_id);

                if (object)
                {
                    if (Input::getSingleton().is_key_pressed(Command::Value::Multiple_Order) == false)
                    {
                        object->get_orders()->remove_all_orders();
                    }

                    auto order = OrderManager::getSingleton().order_enable(Id::Empty, object_id);

                    object->get_orders()->add_last(order->get_id());
                }
            }
        };

        static auto stop_order = []()
        {
            for (const auto& object_id : *(Game::getSingleton().get_active_player()->get_selected()))
            {
                auto object = ObjectManager::getSingleton().get(object_id);

                if (object)
                {
                    if (Input::getSingleton().is_key_pressed(Command::Value::Multiple_Order) == false)
                    {
                        object->get_orders()->remove_all_orders();
                    }

                    auto order = OrderManager::getSingleton().order_stop(Id::Empty, object_id);

                    object->get_orders()->add_last(order->get_id());
                }
            }
        };

        static const std::map<std::string, std::function<void()>> functions =
        {
            { "Destroy", std::bind(destroy_order) },
            { "Disable", std::bind(disable_order) },
            { "Enable", std::bind(enable_order) },
            { "Stop", std::bind(stop_order) }
        };

        if (args.size() == 1 && args[0].IsString())
        {
            auto order = Utils::Convert::to_string(args[0]);
            auto function = functions.find(order);

            if (function != functions.end())
            {
                function->second();
            }
            else
            {
                set_order_name(order_type::from_string(order));
            }
        }
    }

    void UI::engine_ui_set_skill(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args)
    {
        if (args.size() == 1 && args[0].IsString())
        {
            set_skill_name(Utils::Convert::to_string(args[0]));
        }
    }
    */

    bool UI::inject_key_press(Rml::Input::KeyIdentifier key)
    {
        // L_TRACE << "UI::inject_key_press(" << key << ")";

        switch (key)
        {
            case Rml::Input::KeyIdentifier::KI_F1:
                m_document->GetElementById("console")->SetClass("hidden", !m_document->GetElementById("console")->IsClassSet("hidden"));
                return true;

            case Rml::Input::KeyIdentifier::KI_F5:
                init_documents();
                return true;

            case Rml::Input::KeyIdentifier::KI_F8:
                Rml::Debugger::SetVisible(!Rml::Debugger::IsVisible());
                return true;
        }

        return m_context->ProcessKeyDown(key, 0);
    }

    bool UI::inject_key_release(Rml::Input::KeyIdentifier key)
    {
        // L_TRACE << "UI::inject_key_release(" << key << ")";

        return m_context->ProcessKeyUp(key, 0);
    }

    bool UI::inject_text(unsigned int text)
    {
        // L_TRACE << "UI::inject_text(" << text << ")";

        return m_context->ProcessTextInput(static_cast<char>(text));
    }

    bool UI::inject_mouse_move(int x, int y, int z)
    {
        if (z == 0)
        {
            return m_context->ProcessMouseMove(x, y, 0);
        }
        else
        {
            return m_context->ProcessMouseWheel(static_cast<float>(z), 0);
        }
    }

    bool UI::inject_mouse_press(int button)
    {
        return m_context->ProcessMouseButtonDown(button, 0);
    }

    bool UI::inject_mouse_release(int button)
    {
        return m_context->ProcessMouseButtonUp(button, 0);
    }

    bool UI::is_mouse_inside(int x, int y)
    {
        Rml::ElementList elements;

        m_document->GetElementsByClassName(elements, "panel");

        for (Rml::Element* element : elements)
        {
            if (element->IsPointWithinElement(Rml::Vector2f(x, y)))
            {
                return true;
            }
        }

        return false;
    }

    void UI::log_error(const std::string& text, Id id)
    {
        log_write(text, "error", id);
    }

    void UI::log_info(const std::string& text, Id id)
    {
        log_write(text, "info", id);
    }

    void UI::reset()
    {
        reset_configuration();
        reset_order();
        reset_skill();
    }

    void UI::reset_configuration()
    {
        if (get_configuration_name().empty() == false)
        {
            // set_configuration_name("");
        }
    }

    void UI::reset_order()
    {
        if (get_order_type() != order_type::Value::None)
        {
            // set_order_type(order_type::Value::None);
        }
    }

    void UI::reset_skill()
    {
        if (get_skill_name().empty() == false)
        {
            // set_skill_name("");
        }
    }

    void UI::restore_visibility(const std::map<std::string, bool>& state)
    {
        auto cursor_state = state.find("cursor");

        if (cursor_state != state.end())
        {
            // get_cursor().set_visible(cursor_state->second);
        }

        auto minimap_state = state.find("minimap");

        if (minimap_state != state.end())
        {
            // get_minimap().set_visible(minimap_state->second);
        }

        auto preview_state = state.find("preview");

        if (preview_state != state.end())
        {
            // get_preview().set_visible(preview_state->second);
        }

        auto selection_box_state = state.find("selection_box");

        if (selection_box_state != state.end())
        {
            m_selection_box->set_visible(selection_box_state->second);
        }

        auto ui_layers_state = state.find("ui_layers");

        if (ui_layers_state != state.end())
        {
            show_layers(ui_layers_state->second);
        }
    }

    void UI::show_layers(bool visible)
    {
        /*
        for (const auto& [name, surface] : surface_factory->get_surfaces())
        {
        surface->set_visible(visible);
        }
        */
    }

    void UI::show_menu()
    {
        // Update UI.
        // view->EvaluateScript("app.menu.show()");
    }

    void UI::toggle_floating_description()
    {
        m_floating_description = !m_floating_description;
    }

    /*
    void UI::set_visible(bool visible)
    {
        // m_visible = visible;

        // show_layers(visible);

        // get_cursor().set_visible(visible);
        // get_preview().set_visible(visible);
    }

    void UI::set_configuration_name(const std::string& configuration_name)
    {
        reset();

        m_configuration_name = configuration_name;

        if (m_configurations_model)
        {
            m_configurations_model.DirtyVariable("configuration_name");
        }

        TODO: Fix cursor.

        // Update cursor.
        if (configuration_name.empty() == false)
        {
            get_cursor().set_type(Cursor::Type::Square);
        }
        else
        {
            auto configuration = ConfigurationManager::getSingleton().get(configuration_name);

            get_cursor().set_mesh(configuration);
            get_cursor().set_type(Cursor::Type::Object);
        }

        get_cursor().set_visible(false);

        // Reset order.
        if (m_order_type != order_type::Value::None)
        {
            reset_order();
        }
    }

    void UI::set_configurations(const std::set<std::string>& configurations)
    {
        // Update UI.
        m_configurations.clear();

        for (const std::string& i : configurations)
        {
            m_configurations.push_back(i);
        }

        if (m_configurations_model)
        {
            m_configurations_model.DirtyVariable("configurations");
        }
    }

            struct Order
        {
            int         id;
            std::string name;
            std::string sender_name;
            std::string receiver_name;
            std::string attempts;
        };

    void UI::set_diplomacy()
    {
        // Update UI.
        m_diplomacy.clear();

        for (const auto& [id, player] : PlayerManager::getSingleton())
        {
            m_diplomacy.push_back({ id.get(), player->get_name(), player->get_color()});
        }

        if (m_diplomacy_model)
        {
            m_diplomacy_model.DirtyVariable("diplomacy");
        }
    }

    void UI::set_floating_descriptions()
    {
        TODO: Works very slow.

        // Create RML.
        Rml::String rml;

        auto camera = MapManager::getSingleton().begin()->second->get_camera(Settings::Camera::Main)->get_camera();
        auto window = Game::getSingleton().getRenderWindow();

        for (const auto& [id, object] : ObjectManager::getSingleton())
        {
            const Ogre::AxisAlignedBox& box = object->get_scene_node()._getWorldAABB();
            auto coordinates = Utils::get_screenspace_coords(box, camera, true);

            if (coordinates)
            {
                auto width = static_cast<int>(std::roundf(coordinates->x * window->getWidth()));
                auto height = static_cast<int>(std::roundf(coordinates->y * window->getHeight()));

                rml += std::format("<div class=\"floating-description\" id=\"object_{}\" style=\"left: {}px; top: {}px;\">", id.get(), width, height);
                rml += std::format("<div class=\"name\">{}</div>", object->get_name());

                for (const auto& [name, value] : object->get_progress_bars())
                {
                    rml += std::format("<div class=\"bar {}\"><div class=\"bar-background\"><div class=\"bar-current\" style=\"width: {}%;\"></div></div></div>", name, value);
                }

                rml += "</div>";
            }
        }

        // Update UI.
        floating_descriptions_body->SetInnerRML(rml);
    }

    void UI::set_layers(const std::map<std::string, std::shared_ptr<Layer>>& layers)
    {
        TODO: Fix.
        
        // Update cache.
        static std::map<std::string, std::shared_ptr<Layer>> layers_cache;

        if (layers_cache == layers)
        {
            return;
        }

        layers_cache = layers;

        // Create JSON.
        Json::Value json_layers;

        for (const auto& [name, layer] : layers)
        {
            Json::Value json_layer;

            for (const auto& [name, data_layer] : layer->get_data_layers())
            {
                Json::Value json_data_layer;

                json_data_layer["name"] = name;

                json_layer[name] = json_data_layer;
            }

            json_layers[name] = json_layer;
        }

        // Update UI.
        static std::stringstream stream;

        stream.str("");
        stream << "app.layers.set(";
        stream << Utils::Convert::to_string(json_layers);
        stream << ")";
    }

    void UI::set_maps(const std::vector<std::string>& maps)
    {
        // Update cache.
        // TODO: Fix.

        // Update UI.
        m_maps.clear();

        for (const auto& map : maps)
        {
            m_maps.push_back(map);
        }

        if (m_map_menu_model)
        {
            m_map_menu_model.DirtyVariable("maps");
        }
    }
    */

    void UI::set_hovered_object_id(const Id& hovered_object_id)
    {
        m_hovered_object_id = hovered_object_id;
    }

    void UI::set_info(const ConfigurationPtr& info)
    {
        std::string str = info->to_string("  ");

        boost::algorithm::replace_all(str, "\"", "");
        boost::algorithm::replace_all(str, "{", "");
        boost::algorithm::replace_all(str, "}", "");
        boost::algorithm::replace_all(str, " [", "");
        boost::algorithm::replace_all(str, "]", "");
        boost::algorithm::replace_all(str, ",", "");
        boost::algorithm::replace_all(str, " :", ":");

        str = boost::regex_replace(str, boost::regex(" +\n"), "\n");
        str = boost::regex_replace(str, boost::regex("\n+"), "\n");
        str = boost::regex_replace(str, boost::regex("\n  "), "\n");

        m_info->set_description(str);
    }

    /*
    void UI::set_objects_admin()
    {
        // Update UI.
        m_objects_admin.clear();

        for (const auto& [id, object] : ObjectManager::getSingleton())
        {
            std::string order_name;

            if (object->get_orders()->size())
            {
                Order* order = OrderManager::getSingleton().get(object->get_orders()->front());

                if (order)
                {
                    order_name = order_type::to_string(order->get_type());
                }
            }

            m_objects_admin.push_back({ id.get(), object->get_name(), object->get_orders()->size(), order_name });
        }

        if (m_objects_admin_model)
        {
            m_objects_admin_model.DirtyVariable("objects_admin");
        }
    }

    void UI::set_orders_admin()
    {
        // Update UI.
        m_orders_admin.clear();

        for (const auto& [id, order] : OrderManager::getSingleton())
        {
            std::string sender_name;
            
            Object* sender = ObjectManager::getSingleton().get(order->get_sender_id());

            if (sender)
            {
                sender_name = sender->get_name();
            }
            
            std::string receiver_name;
            
            Object* receiver = ObjectManager::getSingleton().get(order->get_receiver_id());

            if (receiver)
            {
                receiver_name = receiver->get_name();
            }

            std::string order_name = order_type::to_string(order->get_type());

            std::string attempts
                = Utils::Convert::to_string(order->get_attempts_to_complete())
                + "/"
                + Utils::Convert::to_string(Settings::Game::OrderMaxAttemptsToComplete);

            m_orders_admin.push_back({ id.get(), sender_name, receiver_name, order_name, attempts });
        }

        if (m_orders_admin_model)
        {
            m_orders_admin_model.DirtyVariable("orders_admin");
        }
    }

    void UI::set_order_type(order_type::Value order_type)
    {
        reset();

        m_order_name = order_type::to_string(order_type);
        m_order_type = order_type;

        if (m_orders_model)
        {
            m_orders_model.DirtyVariable("order_name");
        }
    }

    void UI::set_orders(const std::set<std::string>& orders)
    {
        // Update UI.
        m_orders.clear();

        for (const std::string& i : orders)
        {
            m_orders.push_back(i);
        }

        if (m_orders_model)
        {
            m_orders_model.DirtyVariable("orders");
        }
    }

    void UI::set_resources(const std::shared_ptr<Resources>& resources)
    {
        // Update cache.
        static Resources resources_cache;

        if (resources_cache == (*(resources.get())))
        {
            return;
        }

        resources_cache = (*(resources.get()));

        // Update UI.
        m_resources.clear();

        for (const auto& resource : (*(resources.get())))
        {
            float ratio = resource.second.get_consumption() - resource.second.get_production();

            m_resources.push_back({
                resource.first,
                resource.second.get_current(),
                resource.second.get_max(),
                ratio > 0.0f ? "green" : "red",
                ratio
            });
        }

        if (m_resources_model)
        {
            m_resources_model.DirtyVariable("resources");
        }
    }

    void UI::set_saves(const std::vector<std::string>& saves)
    {
        // Update UI.
        m_saves.clear();

        for (const auto& save : saves)
        {
            m_saves.push_back(save);
        }

        if (m_load_menu_model)
        {
            m_load_menu_model.DirtyVariable("saves");
        }
    }

    void UI::set_skill_name(const std::string& skill_name)
    {
        reset();

        m_skill_name = skill_name;

        if (m_skills_model)
        {
            m_skills_model.DirtyVariable("skill_name");
        }
    }

    void UI::set_skills(const std::set<std::string>& skills)
    {
        // Update UI.
        m_skills.clear();

        for (const std::string& i : skills)
        {
            m_skills.push_back(i);
        }

        if (m_skills_model)
        {
            m_skills_model.DirtyVariable("skills");
        }
    }

    void UI::set_statistics(const std::map<std::string, std::string>& statistics)
    {
        // Update cache.
        static std::map<std::string, std::string> statistics_cache;

        if (statistics_cache == statistics)
        {
            return;
        }

        statistics_cache = statistics;

        // Update UI.
        m_statistics.clear();

        for (const auto& i : statistics)
        {
            m_statistics.push_back({ i.first, i.second });
        }

        if (m_statistics_model)
        {
            m_statistics_model.DirtyVariable("statistics");
        }
    }

    void UI::set_terrain_layers(const std::set<std::string>& layers)
    {
        // Update cache.
        static std::set<std::string> layers_cache;

        if (layers_cache == layers)
        {
            return;
        }

        layers_cache = layers;

        /*
        // Create JSON.
        Json::Value json_layers;

        for (const auto& layer : layers)
        {
            json_layers.append(layer);
        }

        // Update UI.
        static std::stringstream stream;

        stream.str("");
        stream << "app.terrain.set(";
        stream << Utils::Convert::to_string(json_layers);
        stream << ")";
    }

    void UI::set_water_layers(const std::set<std::string>& layers)
    {
        // Update cache.
        static std::set<std::string> layers_cache;

        if (layers_cache == layers)
        {
            return;
        }

        layers_cache = layers;

        // Create JSON.
        Json::Value json_layers;

        for (const auto& layer : layers)
        {
            json_layers.append(layer);
        }

        // Update UI.
        static std::stringstream stream;

        stream.str("");
        stream << "app.water.set(";
        stream << Utils::Convert::to_string(json_layers);
        stream << ")";
    }
    */

    void UI::init_components()
    {
        m_configurations = std::make_shared<ConfigurationsWidget>();
        m_console = std::make_shared<ConsoleWidget>();
        m_cursor = std::make_shared<CursorWidget>();
        m_diplomacy = std::make_shared<DiplomacyWidget>();
        m_info = std::make_shared<InfoWidget>();
        m_log = std::make_shared<LogWidget>();
        m_minimap = std::make_shared<MinimapWidget>();
        m_orders = std::make_shared<OrdersWidget>();
        m_players = std::make_shared<PlayersWidget>();
        m_preview = std::make_shared<PreviewWidget>();
        m_resources = std::make_shared<ResourcesWidget>();
        m_selection_box = std::make_shared<SelectionBoxWidget>();
        m_skills = std::make_shared<SkillsWidget>();
        m_statistics = std::make_shared<StatisticsWidget>();
    }

    void UI::init_data_bindings()
    {
        m_configurations->init_data_bindigs(m_context);
        m_console->init_data_bindigs(m_context);
        m_cursor->init_data_bindigs(m_context);
        m_diplomacy->init_data_bindigs(m_context);
        m_info->init_data_bindigs(m_context);
        m_log->init_data_bindigs(m_context);
        m_minimap->init_data_bindigs(m_context);
        m_orders->init_data_bindigs(m_context);
        m_players->init_data_bindigs(m_context);
        m_preview->init_data_bindigs(m_context);
        m_resources->init_data_bindigs(m_context);
        m_selection_box->init_data_bindigs(m_context);
        m_skills->init_data_bindigs(m_context);
        m_statistics->init_data_bindigs(m_context);

        /*
        if (auto handle = constructor.RegisterStruct<Data_Object>())
        {
            handle.RegisterMember("id", &Data_Object::id);
            handle.RegisterMember("name", &Data_Object::name);
            handle.RegisterMember("order_count", &Data_Object::order_count);
            handle.RegisterMember("order_name", &Data_Object::order_name);
        }

        // Load menu.
        {
            if (auto constructor = m_context->CreateDataModel("load_menu"))
            {
                constructor.Bind("saves", &m_saves);

                m_load_menu_model = constructor.GetModelHandle();
            }
        }

        // Map menu.
        {
            if (auto constructor = m_context->CreateDataModel("map_menu"))
            {
                constructor.Bind("maps", &m_maps);

                m_map_menu_model = constructor.GetModelHandle();
            }
        }

        // Objects admin.
        {
            if (auto constructor = m_context->CreateDataModel("objects_admin"))
            {
                constructor.Bind("objects_admin", &m_objects_admin);

                m_objects_admin_model = constructor.GetModelHandle();
            }
        }

        // Orders admin.
        {
            if (auto constructor = m_context->CreateDataModel("orders_admin"))
            {
                constructor.Bind("orders_admin", &m_orders_admin);

                m_orders_admin_model = constructor.GetModelHandle();
            }
        }

        // Technologies.
        {
            if (auto constructor = m_context->CreateDataModel("technologies"))
            {
                constructor.Bind("technologies", &m_technologies);

                m_technologies_model = constructor.GetModelHandle();
            }
        }
        */
    }

    void UI::init_documents()
    {
        Rml::Debugger::Shutdown();

        m_context->UnloadAllDocuments();

        m_document = m_context->LoadDocument(m_configuration->get_string("layout"));
        m_document->ReloadStyleSheet();
        m_document->Show();

        Rml::Debugger::Initialise(m_context);
    }

    void UI::init_events()
    {
        m_configurations->init_events(m_document);
        m_console->init_events(m_document);
        m_cursor->init_events(m_document);
        m_diplomacy->init_events(m_document);
        m_info->init_events(m_document);
        m_log->init_events(m_document);
        m_minimap->init_events(m_document);
        m_orders->init_events(m_document);
        m_players->init_events(m_document);
        m_preview->init_events(m_document);
        m_resources->init_events(m_document);
        m_selection_box->init_events(m_document);
        m_skills->init_events(m_document);
        m_statistics->init_events(m_document);
    }

    void UI::init_fonts()
    {
        std::set<Rml::String> fonts = m_configuration->get_string_array<std::set<Rml::String>>("fonts");

        for (const Rml::String& font : fonts)
        {
            Rml::LoadFontFace(font, false);
        }
    }

    void UI::init_visibility_types()
    {
        /*
        ObjectAttributes visibility_types;

        for (const auto& [name, configuration] : ConfigurationManager::getSingleton().get_configurations(ConfigurationType::Object))
        {
            visibility_types[configuration->get_string("type")].append(name);
        }

        set_visibility_types(visibility_types);
        */
    }

    void UI::refresh_indicators(const Id& id)
    {
        /*
        m_indicators.clear();

        if (ObjectPtr hovered_object = ObjectManager::getSingleton().get(id))
        {
            int order_number = 1;

            for (const auto& order_id : hovered_object->get_orders()->get_queue())
            {
                OrderPtr order = OrderManager::getSingleton().get(order_id);

                if (order == nullptr)
                {
                    continue;
                }

                std::vector<std::shared_ptr<Indicator>> indicators = order->generate_indicators(order_number);

                if (indicators.size())
                {
                    m_indicators.append_range(indicators);

                    order_number += 1;

                    if (order_number > 3)
                    {
                        break;
                    }
                }
            }
        }
        */
    }

    void UI::log_write(const std::string& message, const std::string& type, const Id& id)
    {
        if (m_log)
        {
            m_log->write(message, type, id);
        }
    }
}
