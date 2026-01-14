#include "Gecko/UI/UI.hpp"

#include "Gecko/Cameras/Camera.hpp"
#include "Gecko/Configuration.hpp"
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
#include "Gecko/Players/Player.hpp"
#include "Gecko/Statistics.hpp"
#include "Gecko/System.hpp"
#include "Gecko/Technologies/TechnologyTree.hpp"
#include "Gecko/UI/Cursor.hpp"
#include "Gecko/UI/EventListenerInstancer.hpp"
#include "Gecko/UI/Minimap.hpp"
#include "Gecko/UI/Preview.hpp"
#include "Gecko/UI/RenderInterface.hpp"
#include "Gecko/UI/SelectionBox.hpp"
#include "Gecko/UI/SystemInterface.hpp"
#include "Gecko/Utils/Convert.hpp"
#include "Gecko/Utils/String.hpp"

Gecko::UI* Ogre::Singleton<Gecko::UI>::msSingleton = nullptr;

namespace Gecko
{
    void UI::init()
    {
        // TODO: Get window size.
        render_interface = std::make_shared<RenderInterface>(1920, 1200);
        system_interface = std::make_shared<SystemInterface>();

        Rml::SetRenderInterface(render_interface.get());
        Rml::SetSystemInterface(system_interface.get());

        Rml::Initialise();

        // TODO: Get window size.
        context = Rml::CreateContext("main", Rml::Vector2i(1920, 1200));

        // TODO: Move to configuration.
        if (false)
        {
            Rml::Debugger::Initialise(context);
        }

        // Load fonts.
        init_fonts();

        // Setup events.
        // DemoEventListenerInstancer event_listener_instancer{ &demo_window };
        // Rml::Factory::RegisterEventListenerInstancer(&event_listener_instancer);

        // demo_window.GetDocument()->AddEventListener(Rml::EventId::Keydown, &demo_window);
        // demo_window.GetDocument()->AddEventListener(Rml::EventId::Keyup, &demo_window);
        // demo_window.GetDocument()->AddEventListener(Rml::EventId::Animationend, &demo_window);

        // Load cursor.
        /*
        Rml::ElementDocument* cursor = context->LoadMouseCursor(getResourceFullPath("cursor.rml").c_str());

        if (cursor)
        {
            // cursor->RemoveReference();
        }
        */

        init_components();
        init_events();
        init_documents();
        init_visibility_types();
    }

    void UI::deinit()
    {
        cursor.reset();
        minimap.reset();
        preview.reset();
        selection_box.reset();

        Rml::Shutdown();
    }

    void UI::update(float time)
    {
        if (refresh_time.update(time))
        {
            refresh_time.reset();

            auto hovered_object = ObjectManager::getSingleton().get(hovered_object_id);

            if (hovered_object)
            {
                set_info(hovered_object->get_info());
            }
            else if (Game::getSingleton().get_active_player() && Game::getSingleton().get_active_player()->get_selected()->size())
            {
                // TODO: Optimize. Get only first.
                for (const auto& id : *(Game::getSingleton().get_active_player()->get_selected()))
                {
                    auto object = ObjectManager::getSingleton().get(id);

                    if (object)
                    {
                        set_info(object->get_info());

                        break;
                    }
                }
            }
            else
            {
                auto memory = System::get_memory_usage();
                auto& game = Game::getSingleton();

                Json::Value info;

                const Ogre::RenderTarget::FrameStats& window_statistics = Game::getSingleton().getRenderWindow()->getStatistics();

                info["Average FPS"] = Utils::Convert::to_string(window_statistics.avgFPS, 2);
                info["Last FPS"] = Utils::Convert::to_string(window_statistics.lastFPS, 2);
                info["Triangles"] = window_statistics.triangleCount;
                info["Active player ID"] = Game::getSingleton().get_active_player_id();
                info["Cursor"] = get_cursor().get_position().to_string();
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

                /*
                const auto& seasons = MapManager::getSingleton().get_items().begin()->second->get_seasons();

                for (const auto& i : seasons)
                {
                    info.emplace(i.get_name(), i.get_current().get_name());
                }
                */

                set_info(std::make_shared<Configuration>(info));

                std::map<std::string, std::string> statistics;

                for (const auto& i : Statistics::getSingleton())
                {
                    statistics.emplace(i.first, Utils::Convert::to_string(i.second));
                }

                set_statistics(statistics);
            }

            // TODO: Optimize.
            // set_floating_descriptions(objects);

            // TODO: First map.
            set_layers(MapManager::getSingleton().begin()->second->get_layers());
            set_maps(Game::getSingleton().get_maps());
            set_objects_admin();
            set_orders_admin();
            set_players();

            if (Game::getSingleton().get_active_player())
            {
                set_resources(Game::getSingleton().get_active_player()->get_resources());
            }

            set_saves(Game::getSingleton().get_saves());

            get_minimap().update(time);
            get_preview().update(time);
        }
    }

    UI::UI(const ConfigurationPtr& configuration) :
        m_configuration(configuration)
    {
    }

    UI::~UI()
    {
    }

    void UI::render(Ogre::uint8 queueGroupId, const Ogre::String& cameraName, bool& skipThisInvocation)
    {
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

        context->Update();

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

        // TODO: Investigate.
        /*
        Ogre::TextureUnitState::UVWAddressingMode addressing_mode;

        addressing_mode.u = Ogre::TextureUnitState::TAM_CLAMP;
        addressing_mode.v = Ogre::TextureUnitState::TAM_CLAMP;
        addressing_mode.w = Ogre::TextureUnitState::TAM_CLAMP;

        render_system->_setTextureAddressingMode(0, addressing_mode);
        */

        render_system->_setTextureCoordSet(0, 0);
        render_system->_setTextureCoordCalculation(0, Ogre::TEXCALC_NONE);

        // TODO: Investigate.
        // render_system->_setTextureUnitFiltering(0, Ogre::FO_LINEAR, Ogre::FO_LINEAR, Ogre::FO_POINT);

        render_system->_setTextureMatrix(0, Ogre::Matrix4::IDENTITY);
        render_system->_setAlphaRejectSettings(Ogre::CMPF_GREATER, 0, false);
        render_system->_disableTextureUnitsFrom(1);
        render_system->_setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);
        render_system->_setDepthBias(0, 0);

        context->Render();
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

                    // TODO: Throw exception.

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

                    // TODO: Throw exception.

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

                    // TODO: Throw exception.

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

                    // TODO: Throw exception.

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

    void UI::inject_key_press(char key_code)
    {
        // TODO: Fill the last argument.
        context->ProcessKeyDown(Utils::Convert::to_rmlui_key(key_code), 0);

        switch (Utils::Convert::to_rmlui_key(key_code))
        {
            case Rml::Input::KeyIdentifier::KI_F5:
                init_documents();
                break;

            case Rml::Input::KeyIdentifier::KI_F8:
                Rml::Debugger::SetVisible(!Rml::Debugger::IsVisible());
                break;
        }
    }

    void UI::inject_key_release(char key_code)
    {
        // TODO: Fill the last argument.
        context->ProcessKeyUp(Utils::Convert::to_rmlui_key(key_code), 0);
    }

    void UI::inject_mouse_move(std::size_t x, std::size_t y)
    {
        // TODO: Convert types and fill the last argument.
        context->ProcessMouseMove(x, y, 0);
    }

    void UI::inject_mouse_press(std::size_t x, std::size_t y, OIS::MouseButtonID id)
    {
        // TODO: Fill the last argument.
        context->ProcessMouseButtonDown(Utils::Convert::to_rmlui_button(id), 0);
    }

    void UI::inject_mouse_release(std::size_t x, std::size_t y, OIS::MouseButtonID id)
    {
        // TODO: Fill the last argument.
        context->ProcessMouseButtonUp(Utils::Convert::to_rmlui_button(id), 0);
    }

    bool UI::is_mouse_inside(std::size_t x, std::size_t y)
    {
        Rml::ElementList elements;

        document->GetElementsByClassName(elements, "card");

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
        if (get_configuration_name() != "none")
        {
            set_configuration_name("none");
        }
    }

    void UI::reset_order()
    {
        if (get_order_name() != order_type::Value::None)
        {
            set_order_name(order_type::Value::None);
        }
    }

    void UI::reset_skill()
    {
        if (get_skill_name() != "none")
        {
            set_skill_name("none");
        }
    }

    void UI::restore_visibility(const std::map<std::string, bool>& state)
    {
        auto cursor_state = state.find("cursor");

        if (cursor_state != state.end())
        {
            get_cursor().set_visible(cursor_state->second);
        }

        auto minimap_state = state.find("minimap");

        if (minimap_state != state.end())
        {
            // get_minimap().set_visible(minimap_state->second);
        }

        auto preview_state = state.find("preview");

        if (preview_state != state.end())
        {
            get_preview().set_visible(preview_state->second);
        }

        auto selection_box_state = state.find("selection_box");

        if (selection_box_state != state.end())
        {
            selection_box->set_visible(selection_box_state->second);
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
        floating_description = !floating_description;
    }

    void UI::set_visible(bool visible)
    {
        m_visible = visible;

        show_layers(visible);

        get_cursor().set_visible(visible);
        get_preview().set_visible(visible);
    }

    void UI::set_configuration_name(const std::string& _configuration_name)
    {
        reset();

        configuration_name = _configuration_name;

        // Update UI.
        set_configurations_header(_configuration_name);

        // Update cursor.
        if (_configuration_name == "none")
        {
            get_cursor().set_type(Cursor::Type::Square);
        }
        else
        {
            auto configuration = ConfigurationManager::getSingleton().get(_configuration_name);

            get_cursor().set_mesh(configuration);
            get_cursor().set_type(Cursor::Type::Object);
        }

        get_cursor().set_visible(false);

        // Reset order.
        if (order_name != order_type::Value::None)
        {
            reset_order();
        }
    }

    void UI::set_configurations(const std::set<std::string>& configurations)
    {
        // TODO: Remove
        if (configurations_body == nullptr)
        {
            return;
        }

        // Update cache.
        static std::set<std::string> configurations_cache;

        if (configurations_cache == configurations)
        {
            return;
        }

        configurations_cache = configurations;

        // Create RML.
        Rml::String rml;

        for (const std::string& _configuration : configurations)
        {
            std::string title = Utils::String::to_title(_configuration);

            rml += std::vformat(m_configuration->get_string("templates.configurations"), std::make_format_args(_configuration, title));
        }

        // Update UI.
        configurations_body->SetInnerRML(rml);
    }

    void UI::set_configurations_header(const std::string& _configuration_name)
    {
        // Update cache.
        static std::string configuration_name_cache = "none";

        if (configuration_name_cache == _configuration_name)
        {
            return;
        }

        // Update UI.
        configurations_header->SetInnerRML(Utils::String::to_title(_configuration_name));
    }

    void UI::set_floating_descriptions(const std::vector<Id>& objects)
    {
        // TODO: Refactor? Fix? Remove?
        /*
        // Update cache.
        static std::vector<Id> json_floating_descriptions_cache;

        if (json_floating_descriptions_cache == objects)
        {
            return;
        }

        json_floating_descriptions_cache = objects;

        // Create JSON.
        Json::Value json_floating_descriptions;

        auto camera = Game::getSingleton().get_camera(Settings::Camera::MainName)->get_camera();
        auto window = Game::getSingleton().get_window(Settings::Window::MainName);

        for (const auto& id : objects)
        {
            auto object = ObjectManager::getSingleton().get(id);
            auto coordinates = Utils::get_screenspace_coords(object->get_mesh().get_entity(), camera, true);

            if (coordinates)
            {
                auto width = static_cast<int>(std::roundf(coordinates->x * window->get_width()));
                auto height = static_cast<int>(std::roundf(coordinates->y * window->get_height()));

                Json::Value json_object;

                json_object["id"] = id;
                json_object["name"] = object->get_name();
                json_object["left"] = width;
                json_object["top"] = height;

                for (const auto& [name, value] : object->get_progress_bars())
                {
                    json_object["bars"][name] = value;
                }

                json_floating_descriptions.append(json_object);
            }
        }

        // Update UI.
        static std::stringstream stream;

        stream.str("");
        stream << "app.ui.set_floating_descriptions(";
        stream << Utils::Convert::to_string(json_floating_descriptions);
        stream << ")";
        */
    }

    void UI::set_layers(const std::map<std::string, std::shared_ptr<Layer>>& layers)
    {
        /*
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
        */
    }

    void UI::set_maps(const std::vector<std::string>& maps)
    {
        /*
        TODO: Fix.

        // Update cache.
        static std::vector<std::string> maps_cache;

        if (maps_cache == maps)
        {
            return;
        }

        maps_cache = maps;

        // Create JSON.
        Json::Value json_maps;

        for (const auto& i : maps)
        {
            json_maps.append(i);
        }

        // Update UI.
        static std::stringstream stream;

        stream.str("");
        stream << "app.map_menu.set_maps(";
        stream << Utils::Convert::to_string(json_maps);
        stream << ")";
        */
    }

    void UI::set_hovered_object_id(Id object_id)
    {
        hovered_object_id = object_id;
    }

    void UI::set_info(const ConfigurationPtr& info)
    {
        // Update cache.
        static ConfigurationPtr info_cache;

        if (info_cache == info)
        {
            return;
        }

        info_cache = info;

        // Create RML.
        std::string rml = info->to_string("  ");

        boost::algorithm::replace_all(rml, "\"", "");
        boost::algorithm::replace_all(rml, "{", "");
        boost::algorithm::replace_all(rml, "}", "");
        boost::algorithm::replace_all(rml, " [", "");
        boost::algorithm::replace_all(rml, "]", "");
        boost::algorithm::replace_all(rml, ",", "");
        boost::algorithm::replace_all(rml, " :", ":");

        rml = boost::regex_replace(rml, boost::regex(" +\n"), "\n");
        rml = boost::regex_replace(rml, boost::regex("\n+"), "\n");
        rml = boost::regex_replace(rml, boost::regex("\n  "), "\n");

        // Update UI.
        info_body->SetInnerRML(rml);
    }

    void UI::set_objects_admin()
    {
        // TODO: Fix cache.
        /*
        // Update cache.
        static ObjectManager::Container objects_cache;

        if (objects_cache == objects)
        {
            return;
        }

        objects_cache = objects;
        */

        // Create RML.
        Rml::String rml;

        for (const auto& [id, object] : ObjectManager::getSingleton())
        {
            int id_value = id.get();
            std::string name = object->get_name();
            std::size_t order_count = object->get_orders()->size();
            std::string order_name;

            if (object->get_orders()->size())
            {
                Order* order = OrderManager::getSingleton().get(object->get_orders()->front());

                if (order)
                {
                    order_name = order_type::to_string(order->get_type());
                }
            }

            rml += std::vformat(m_configuration->get_string("templates.objects-admin"), std::make_format_args(id_value, name, order_count, order_name));
        }

        // Update UI.
        objects_admin_body->SetInnerRML(rml);
    }

    void UI::set_orders_admin()
    {
        /* TODO: Fix cache.
        // Update cache.
        static OrderManager::Container orders_cache;

        if (orders_cache == orders)
        {
            return;
        }

        orders_cache = orders;
        */

        // Create RML.
        Rml::String rml;

        for (const auto& [id, order] : OrderManager::getSingleton())
        {
            int id_value = id.get();
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

            rml += std::vformat(m_configuration->get_string("templates.orders-admin"), std::make_format_args(id_value, sender_name, receiver_name, order_name, attempts));
        }

        // Update UI.
        orders_admin_body->SetInnerRML(rml);
    }

    void UI::set_players()
    {
        // TODO: Fix cache.
        /*
        // Update cache.
        static PlayerManager::Container players_cache;

        if (players_cache == players)
        {
            return;
        }

        players_cache = players;
        */

        // Create RML.
        Rml::String rml;

        for (const auto& [id, player] : PlayerManager::getSingleton())
        {
            int id_value = id.get();
            std::string name = player->get_name();
            std::string color = player->get_color();

            rml += std::vformat(m_configuration->get_string("templates.players"), std::make_format_args(id_value, name, color));
        }

        // Update UI.
        players_body->SetInnerRML(rml);
    }

    void UI::set_order_name(order_type::Value _order_name)
    {
        reset();

        order_name = _order_name;

        // Update UI.
        set_orders_header(order_name);
    }

    void UI::set_orders(const std::set<std::string>& orders)
    {
        // TODO: Remove.
        if (orders_body == nullptr)
        {
            return;
        }

        // Update cache.
        static std::set<std::string> orders_cache;

        if (orders_cache == orders)
        {
            return;
        }

        orders_cache = orders;

        // Create RML.
        Rml::String rml;

        for (const std::string& order : orders)
        {
            std::string title = Utils::String::to_title(order);

            rml += std::vformat(m_configuration->get_string("templates.orders"), std::make_format_args(order, title));
        }

        // Update UI.
        orders_body->SetInnerRML(rml);
    }

    void UI::set_orders_header(order_type::Value order_type)
    {
        // Update cache.
        static order_type::Value order_type_cache = order_type::Value::None;

        if (order_type_cache == order_type)
        {
            return;
        }

        order_type_cache = order_type;

        // Update UI.
        orders_header->SetInnerRML(Utils::String::to_title(order_type::to_string(order_type)));
    }

    void UI::set_resources(std::shared_ptr<Resources> resources)
    {
        // Update cache.
        static Resources resources_cache;

        if (resources_cache == (*(resources.get())))
        {
            return;
        }

        resources_cache = (*(resources.get()));

        // Create RML.
        std::string rml;

        for (const auto& resource : (*(resources.get())))
        {
            float current = resource.second.get_current();
            float max = resource.second.get_max();
            float ratio = resource.second.get_consumption() - resource.second.get_production();

            std::string class_name = (resource.second.get_consumption() - resource.second.get_production()) > 0.0f ? "green" : "red";

            rml += std::vformat(m_configuration->get_string("templates.resources"), std::make_format_args(resource.first, current, max, class_name, ratio));
        }

        // Update UI.
        resources_body->SetInnerRML(rml);
    }

    void UI::set_saves(const std::vector<std::string>& saves)
    {
        /*
        TODO: Fix.

        // Update cache.
        static std::vector<std::string> saves_cache;

        if (saves_cache == saves)
        {
            return;
        }

        saves_cache = saves;

        // Create JSON.
        Json::Value json_saves;

        for (const auto& i : saves)
        {
            json_saves.append(i);
        }

        // Update UI.
        static std::stringstream stream;

        stream.str("");
        stream << "app.load_menu.set_saves(";
        stream << Utils::Convert::to_string(json_saves);
        stream << ")";
        */
    }

    void UI::set_skill_name(const std::string& _skill_name)
    {
        reset();

        skill_name = _skill_name;

        // Update UI.
        set_skills_header(skill_name);
    }

    void UI::set_skills(const std::set<std::string>& skills)
    {
        // Update cache.
        static std::set<std::string> skills_cache;

        if (skills_cache == skills)
        {
            return;
        }

        skills_cache = skills;

        // Create RML.
        Rml::String rml;

        for (const std::string& skill : skills)
        {
            std::string title = Utils::String::to_title(skill);

            rml += std::vformat(m_configuration->get_string("templates.skills"), std::make_format_args(skill, title));
        }

        // Update UI.
        skills_body->SetInnerRML(rml);
    }

    void UI::set_skills_header(const std::string& skill_name)
    {
        // Update cache.
        static std::string skill_name_cache = "none";

        if (skill_name_cache == skill_name)
        {
            return;
        }

        skill_name_cache = skill_name;

        // Update UI.
        orders_header->SetInnerRML(Utils::String::to_title(skill_name));
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

        // Create RML.
        Rml::String rml;

        for (const auto& i : statistics)
        {
            std::string name  = i.first;
            std::string value = i.second;

            rml += std::vformat(m_configuration->get_string("templates.statistics"), std::make_format_args(name, value));
        }

        // Update UI.
        statistics_body->SetInnerRML(rml);
    }

    void UI::set_terrain_layers(const std::set<std::string>& layers)
    {
        /*
        TODO: Fix.

        // Update cache.
        static std::set<std::string> layers_cache;

        if (layers_cache == layers)
        {
            return;
        }

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
        */
    }

    void UI::set_water_layers(const std::set<std::string>& layers)
    {
        /*
        TODO: Fix.

        // Update cache.
        static std::set<std::string> layers_cache;

        if (layers_cache == layers)
        {
            return;
        }

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
        */
    }

    void UI::init_components()
    {
        cursor = std::make_unique<Cursor>();
        minimap = std::make_unique<Minimap>();
        preview = std::make_unique<Preview>();
        selection_box = std::make_unique<SelectionBox>();
    }

    void UI::init_documents()
    {
        // TODO: Warning on first call.
        Rml::Debugger::Shutdown();

        context->UnloadAllDocuments();

        document = context->LoadDocument(m_configuration->get_string("layout"));
        document->Show();

        Rml::Debugger::Initialise(context);

        configurations_body = get_body_placeholder("configurations");
        configurations_header = get_header_placeholder("configurations");
        info_body = get_body_placeholder("info");
        layers_body = get_body_placeholder("layers");
        log_body = get_body_placeholder("log");
        maps_body = get_body_placeholder("maps");
        objects_admin_body = get_body_placeholder("objects-admin");
        orders_admin_body = get_body_placeholder("orders-admin");
        orders_body = get_body_placeholder("orders");
        orders_header = get_header_placeholder("orders");
        players_body = get_body_placeholder("players");
        resources_body = get_body_placeholder("resources");
        skills_body = get_body_placeholder("skills");
        skills_header = get_header_placeholder("skills");
        statistics_body = get_body_placeholder("statistics");
    }

    void UI::init_events()
    {
        event_listener_instancer = std::make_shared<EventListenerInstancer>();

    	Rml::Factory::RegisterEventListenerInstancer(event_listener_instancer.get());
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

    void UI::log_write(const std::string& text, const std::string& type, Id id)
    {
        // TODO: Refactor. Make class.
        // TODO: Implement id argument.
        static std::vector<std::string> lines;

        lines.push_back(text);

        if (lines.size() > 3)
        {
            lines.erase(lines.cbegin(), lines.cbegin() + (lines.size() - 3));
        }

        if (document)
        {
            std::string rml;

            for (const std::string& line : lines)
            {
                rml += std::vformat(m_configuration->get_string("templates.log"), std::make_format_args(type, line));
            }

            log_body->SetInnerRML(rml);
        }
    }

    Rml::Element* UI::get_header_placeholder(const std::string& selector) const
    {
        Rml::Element* element = document->GetElementById(selector);

        if (element == nullptr)
        {
            return nullptr;
        }

        Rml::ElementList elements;

        element->GetElementsByClassName(elements, "card-header-placeholder");

        if (elements.size() == 0)
        {
            return nullptr;
        }

        return elements[0];
    }

    Rml::Element* UI::get_body_placeholder(const std::string& selector) const
    {
        Rml::Element* element = document->GetElementById(selector);

        if (element == nullptr)
        {
            return nullptr;
        }

        Rml::ElementList elements;

        // TODO: Rename class to card-body-placeholder.
        element->GetElementsByClassName(elements, "placeholder");

        if (elements.size() == 0)
        {
            return nullptr;
        }

        return elements[0];
    }
}
