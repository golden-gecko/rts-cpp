#include "Gecko/UI/UI.hpp"

#include "Gecko/Cameras/Camera.hpp"
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
#include "Gecko/UI/Minimap.hpp"
#include "Gecko/UI/Preview.hpp"
#include "Gecko/UI/SelectionBox.hpp"
#include "Gecko/Utils/Convert.hpp"
#include "Gecko/Window.hpp"

Gecko::UI* Ogre::Singleton<Gecko::UI>::msSingleton = nullptr;

namespace
{
    ultralight::MouseEvent::Button convert_button_id(OIS::MouseButtonID id)
    {
        switch (id)
        {
            case OIS::MouseButtonID::MB_Middle:
                return ultralight::MouseEvent::Button::kButton_Middle;

            case OIS::MouseButtonID::MB_Right:
                return ultralight::MouseEvent::Button::kButton_Right;

            default:
                return ultralight::MouseEvent::Button::kButton_Left;
        }
    }
}

namespace Gecko
{
    void UI::OnFinishLoading(ultralight::View* caller, uint64_t frame_id, bool is_main_frame, const ultralight::String& url)
    {
        is_loaded = true;
    }

    void UI::OnDOMReady(ultralight::View* caller, uint64_t frame_id, bool is_main_frame, const ultralight::String& url)
    {
        using namespace ultralight;

        // SetJSContext(caller->LockJSContext().get());

        auto global = JSGlobalObject();

        global["engine_application_save_options"] = BindJSCallback(&UI::engine_application_save_options);
        global["engine_application_quit"] = BindJSCallback(&UI::engine_application_quit);
        global["engine_game_load"] = BindJSCallback(&UI::engine_game_load);
        global["engine_game_new"] = BindJSCallback(&UI::engine_game_new);
        global["engine_game_save"] = BindJSCallback(&UI::engine_game_save);
        global["engine_game_quit"] = BindJSCallback(&UI::engine_game_quit);
        global["engine_map_set_data_layer"] = BindJSCallback(&UI::engine_map_set_data_layer);
        global["engine_minimap_click"] = BindJSCallback(&UI::engine_minimap_click);
        global["engine_minimap_move"] = BindJSCallback(&UI::engine_minimap_move);
        global["engine_minimap_zoom_in"] = BindJSCallback(&UI::engine_minimap_zoom_in);
        global["engine_minimap_zoom_out"] = BindJSCallback(&UI::engine_minimap_zoom_out);
        global["engine_preview_hide"] = BindJSCallback(&UI::engine_preview_hide);
        global["engine_preview_set_position"] = BindJSCallback(&UI::engine_preview_set_position);
        global["engine_preview_show"] = BindJSCallback(&UI::engine_preview_show);
        global["engine_technology_research"] = BindJSCallback(&UI::engine_technology_research);
        global["engine_ui_change_visibility"] = BindJSCallback(&UI::engine_ui_change_visibility);
        global["engine_ui_look_at_object"] = BindJSCallback(&UI::engine_ui_look_at_object);
        global["engine_ui_select_object"] = BindJSCallback(&UI::engine_ui_select_object);
        global["engine_ui_set_configuration"] = BindJSCallback(&UI::engine_ui_set_configuration);
        global["engine_ui_set_order"] = BindJSCallback(&UI::engine_ui_set_order);
        global["engine_ui_set_skill"] = BindJSCallback(&UI::engine_ui_set_skill);

        is_dom_ready = true;
    }

    void UI::LogMessage(ultralight::LogLevel log_level, const ultralight::String& message)
    {
        switch (log_level)
        {
            case ultralight::LogLevel::Error:
                L_ERROR << Utils::Convert::to_string(message);
                break;

            case ultralight::LogLevel::Info:
                L_INFO << Utils::Convert::to_string(message);
                break;

            case ultralight::LogLevel::Warning:
                L_WARNING << Utils::Convert::to_string(message);
                break;

            default:
                L_DEBUG << Utils::Convert::to_string(message);
        }
    }

    void UI::OnAddConsoleMessage(ultralight::View* caller, const ultralight::ConsoleMessage& message)
    {
        L_ERROR
            << "[Console]: [" << Utils::Convert::to_string(message.source())
            << "] [" << Utils::Convert::to_string(message.level())
            << "] " << Utils::Convert::to_string(message.message());

        if (message.source() == ultralight::kMessageSource_JS)
        {
            L_ERROR
                << " (" << Utils::Convert::to_string(message.source_id())
                << " @ line " << Utils::Convert::to_string(message.line_number())
                << ", col " << Utils::Convert::to_string(message.column_number())
                << ")";
        }
    }

    UI::UI(const std::shared_ptr<Configuration>& configuration) :
        configuration(configuration)
    {
    }

    UI::~UI()
    {
        view = nullptr;
        renderer = nullptr;

        ogre_surface.reset();
    }

    void UI::init()
    {
        /*
        auto width = configuration->get_int<std::size_t>("ui.width", 1024);
        auto height = configuration->get_int<std::size_t>("ui.height", 768);

        auto scale = configuration->get_float("ui.scale", 1.0);
        auto font = configuration->get_string("ui.font", "Arial");

        auto current_directory = std::filesystem::path(std::filesystem::current_path());
        auto layout = std::filesystem::path(configuration->get_string("ui.layout"));
        auto url = "file:///" + (current_directory / layout).string();

        // Create texture.
        ogre_surface = std::make_unique<OgreSurface>("ui", width, height);

        // Create configuration.
        ultralight::Config config;

        config.device_scale = scale;
        config.font_family_fixed = font.c_str();
        config.font_family_serif = font.c_str();
        config.font_family_sans_serif = font.c_str();
        config.font_family_standard = font.c_str();
        config.font_hinting = ultralight::FontHinting::kFontHinting_Smooth;
        config.use_gpu_renderer = false;

        ultralight::Platform::instance().set_config(config);
        ultralight::Platform::instance().set_font_loader(ultralight::GetPlatformFontLoader());
        ultralight::Platform::instance().set_file_system(ultralight::GetPlatformFileSystem("."));
        ultralight::Platform::instance().set_logger(this);

        // Create renderer.
        renderer = ultralight::Renderer::Create();

        // Create view.
        ultralight::ViewConfig view_config;
        
        view_config.initial_device_scale = 2.0;
        view_config.is_accelerated = false;

        view = renderer->CreateView(width, height, view_config, nullptr);
        view->set_load_listener(this);
        view->set_view_listener(this);
        view->LoadURL(url.c_str());

        init_components();
        init_visibility_types();

        wait_until_ready();
        */
    }

    void UI::deinit()
    {
        cursor.reset();
        minimap.reset();
        preview.reset();
        selection_box.reset();

        ogre_surface.reset();
    }

    void UI::update(float time)
    {
        if (refresh_time.update(time))
        {
            refresh_time.reset();

            auto hovered_object = ObjectManager::getSingleton().get(hovered_object_id);

            if (hovered_object)
            {
                set_info(hovered_object->serialize());
            }
            else if (Game::getSingleton().get_active_player() && Game::getSingleton().get_active_player()->get_selected()->size())
            {
                // TODO: Optimize. Get only first.
                for (const auto& id : *(Game::getSingleton().get_active_player()->get_selected()))
                {
                    auto object = ObjectManager::getSingleton().get(id);

                    if (object)
                    {
                        set_info(object->serialize());

                        break;
                    }
                }
            }
            else
            {
                auto memory = System::get_memory_usage();
                auto& game = Game::getSingleton();

                Json::Value info;

                if (game.get_windows().empty() == false)
                {
                    const auto& window_statistics = game.get_windows().begin()->second->get_statistics();

                    info["Average FPS"] = Utils::Convert::to_string(window_statistics.avgFPS, 2);
                    info["Last FPS"] = Utils::Convert::to_string(window_statistics.lastFPS, 2);
                    info["Triangles"] = window_statistics.triangleCount;
                }

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
            auto& game = Game::getSingleton();

            // TODO: First map.
            set_layers(MapManager::getSingleton().begin()->second->get_layers());
            set_maps(game.get_maps());
            set_objects_admin();
            set_orders_admin();
            set_players();

            if (Game::getSingleton().get_active_player())
            {
                // TODO: Fix.
                // set_resources(Game::getSingleton().get_active_player()->get_resources());
            }

            set_saves(Game::getSingleton().get_saves());

            get_minimap().update();
            get_preview().update();

            renderer->Update();
            renderer->Render();

            auto bitmap_surface = dynamic_cast<ultralight::BitmapSurface*>(view->surface());
            auto bitmap = bitmap_surface->bitmap();

            if (bitmap_surface->dirty_bounds().IsEmpty() == false)
            {
                ogre_surface->update(bitmap);
            }
        }
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

    void UI::inject_key_press(char key_code)
    {
        // TODO: Implement.
    }

    void UI::inject_key_release(char key_code)
    {
        // TODO: Implement.
    }

    void UI::inject_mouse_move(std::size_t x, std::size_t y)
    {
        ultralight::MouseEvent evt;

        evt.type = ultralight::MouseEvent::kType_MouseMoved;
        evt.x = x;
        evt.y = y;

        view->FireMouseEvent(evt);
    }

    void UI::inject_mouse_press(std::size_t x, std::size_t y, OIS::MouseButtonID id)
    {
        ultralight::MouseEvent evt;

        evt.type = ultralight::MouseEvent::kType_MouseDown;
        evt.x = x;
        evt.y = y;
        evt.button = convert_button_id(id);

        view->FireMouseEvent(evt);
    }

    void UI::inject_mouse_release(std::size_t x, std::size_t y, OIS::MouseButtonID id)
    {
        ultralight::MouseEvent evt;

        evt.type = ultralight::MouseEvent::kType_MouseUp;
        evt.x = x;
        evt.y = y;
        evt.button = convert_button_id(id);

        view->FireMouseEvent(evt);
    }

    bool UI::is_mouse_inside(std::size_t x, std::size_t y)
    {
        if (is_ready() == false)
        {
            return false;
        }

        static std::stringstream stream;

        stream.str("");
        stream << "app.ui.is_mouse_inside(";
        stream << x;
        stream << ", ";
        stream << y;
        stream << ")";

        auto value = view->EvaluateScript(stream.str().c_str());

        return Utils::Convert::to_string(value) == "true";
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
            get_minimap().set_visible(minimap_state->second);
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
        if (is_ready() == false)
        {
            return;
        }

        // Update UI.
        view->EvaluateScript("app.menu.show()");
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
        get_minimap().set_visible(visible);
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
        if (is_ready() == false)
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

        // Create JSON.
        Json::Value json_configurations;

        for (const auto& configuration : configurations)
        {
            json_configurations.append(configuration);
        }

        // Update UI.
        static std::stringstream stream;

        stream.str("");
        stream << "app.configurations.set(";
        stream << Utils::Convert::to_string(json_configurations);
        stream << ")";

        evaluate_with_timeout(stream.str());
    }

    void UI::set_configurations_header(const std::string& _configuration_name)
    {
        if (is_ready() == false)
        {
            return;
        }

        // Update cache.
        static std::string configuration_name_cache = "none";

        if (configuration_name_cache == _configuration_name)
        {
            return;
        }

        // Update UI.
        static std::stringstream stream;

        stream.str("");
        stream << "app.configurations.set_header('";
        stream << _configuration_name;
        stream << "')";

        evaluate_with_timeout(stream.str());
    }

    void UI::set_floating_descriptions(const std::vector<Id>& objects)
    {
        // TODO: Refactor? Fix? Remove?
        /*
        if (is_ready() == false)
        {
            return;
        }

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

        evaluate_with_timeout(stream.str());
        */
    }

    void UI::set_layers(const std::map<std::string, std::shared_ptr<Layer>>& layers)
    {
        if (is_ready() == false)
        {
            return;
        }

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

        evaluate_with_timeout(stream.str());
    }

    void UI::set_maps(const std::vector<std::string>& maps)
    {
        if (is_ready() == false)
        {
            return;
        }

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

        evaluate_with_timeout(stream.str());
    }

    void UI::set_hovered_object_id(Id object_id)
    {
        hovered_object_id = object_id;
    }

    void UI::set_info(const std::shared_ptr<Configuration>& info)
    {
        if (is_ready() == false)
        {
            return;
        }

        // Update cache.
        static std::shared_ptr<Configuration> info_cache;

        if (info_cache == info)
        {
            return;
        }

        info_cache = info;

        // Update UI.
        static std::stringstream stream;

        stream.str("");
        stream << "app.info.set(";
        stream << info_cache->to_string();
        stream << ")";

        evaluate_with_timeout(stream.str());
    }

    void UI::set_objects_admin()
    {
        if (is_ready() == false)
        {
            return;
        }

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

        // Create JSON.
        Json::Value json_objects;

        for (const auto& [id, object] : ObjectManager::getSingleton())
        {
            Json::Value json_object;

            json_object["id"] = id;
            json_object["name"] = object->get_name();
            json_object["order_count"] = object->get_orders()->size();

            if (object->get_orders()->size())
            {
                auto order = OrderManager::getSingleton().get(object->get_orders()->front());

                if (order)
                {
                    json_object["current_order"] = order_type::to_string(order->get_type());
                }
                else
                {
                    json_object["current_order"] = "-";
                }
            }
            else
            {
                json_object["current_order"] = "-";
            }

            json_objects.append(json_object);
        }

        // Update UI.
        static std::stringstream stream;

        stream.str("");
        stream << "app.objects.set(";
        stream << Utils::Convert::to_string(json_objects);
        stream << ")";

        evaluate_with_timeout(stream.str());
    }

    void UI::set_orders_admin()
    {
        if (is_ready() == false)
        {
            return;
        }

        /* TODO: Fix cache.
        // Update cache.
        static OrderManager::Container orders_cache;

        if (orders_cache == orders)
        {
            return;
        }

        orders_cache = orders;
        */

        // Create JSON.
        Json::Value json_orders;

        for (const auto& [id, order] : OrderManager::getSingleton())
        {
            Json::Value json_order;

            json_order["id"] = id;
            json_order["sender_id"] = order->get_sender_id();
            json_order["receiver_id"] = order->get_receiver_id();
            json_order["type"] = order_type::to_string(order->get_type());
            json_order["attempts_to_complete"]
                = Utils::Convert::to_string(order->get_attempts_to_complete())
                + "/"
                + Utils::Convert::to_string(Settings::Game::OrderMaxAttemptsToComplete);

            json_orders.append(json_order);
        }

        // Update UI.
        static std::stringstream stream;

        stream.str("");
        stream << "app.orders.set(";
        stream << Utils::Convert::to_string(json_orders);
        stream << ")";

        evaluate_with_timeout(stream.str());
    }

    void UI::set_players()
    {
        if (is_ready() == false)
        {
            return;
        }

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

        // Create JSON.
        Json::Value json_players;

        for (const auto& [id, player] : PlayerManager::getSingleton())
        {
            Json::Value json_player;

            json_player["id"] = id;
            json_player["name"] = player->get_name();
            json_player["color"] = player->get_color();

            json_players.append(json_player);
        }

        // Update UI.
        static std::stringstream stream;

        stream.str("");
        stream << "app.players.set(";
        stream << Utils::Convert::to_string(json_players);
        stream << ")";

        evaluate_with_timeout(stream.str());
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
        if (is_ready() == false)
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

        // Create JSON.
        Json::Value json_orders;

        for (const auto& order : orders)
        {
            json_orders.append(order);
        }

        // Update UI.
        static std::stringstream stream;

        stream.str("");
        stream << "app.orders.set(";
        stream << Utils::Convert::to_string(json_orders);
        stream << ")";

        evaluate_with_timeout(stream.str());
    }

    void UI::set_orders_header(order_type::Value order_type)
    {
        if (is_ready() == false)
        {
            return;
        }

        // Update cache.
        static order_type::Value order_type_cache = order_type::Value::None;

        if (order_type_cache == order_type)
        {
            return;
        }

        order_type_cache = order_type;

        // Update UI.
        static std::stringstream stream;

        stream.str("");
        stream << "app.orders.set_header('";
        stream << order_type::to_string(order_type);
        stream << "')";

        evaluate_with_timeout(stream.str());
    }

    void UI::set_resources(const Resources& resources)
    {
        if (is_ready() == false)
        {
            return;
        }

        // Update cache.
        static Resources resources_cache;

        if (resources_cache == resources)
        {
            return;
        }

        resources_cache = resources;

        // Create JSON.
        Json::Value json_resources;

        for (const auto& resource : resources)
        {
            Json::Value json_resource;

            json_resource["name"] = resource.first;
            json_resource["consumption"] = resource.second.get_consumption();
            json_resource["production"] = resource.second.get_production();
            json_resource["current"] = resource.second.get_current();
            json_resource["maximal"] = resource.second.get_max();

            json_resources.append(json_resource);
        }

        // Update UI.
        static std::stringstream stream;

        stream.str("");
        stream << "app.resources.set(";
        stream << Utils::Convert::to_string(json_resources);
        stream << ")";

        evaluate_with_timeout(stream.str());
    }

    void UI::set_saves(const std::vector<std::string>& saves)
    {
        if (is_ready() == false)
        {
            return;
        }

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

        evaluate_with_timeout(stream.str());
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
        if (is_ready() == false)
        {
            return;
        }

        // Update cache.
        static std::set<std::string> skills_cache;

        if (skills_cache == skills)
        {
            return;
        }

        skills_cache = skills;

        // Create JSON.
        Json::Value json_skills;

        for (const auto& i : skills)
        {
            json_skills.append(i);
        }

        // Update UI.
        static std::stringstream stream;

        stream.str("");
        stream << "app.skills.set(";
        stream << Utils::Convert::to_string(json_skills);
        stream << ")";

        evaluate_with_timeout(stream.str());
    }

    void UI::set_skills_header(const std::string& skill_name)
    {
        if (is_ready() == false)
        {
            return;
        }

        // Update cache.
        static std::string skill_name_cache = "none";

        if (skill_name_cache == skill_name)
        {
            return;
        }

        skill_name_cache = skill_name;

        // Update UI.
        static std::stringstream stream;

        stream.str("");
        stream << "app.skills.set_header('";
        stream << skill_name;
        stream << "')";

        evaluate_with_timeout(stream.str());
    }

    void UI::set_statistics(const std::map<std::string, std::string>& statistics)
    {
        if (is_ready() == false)
        {
            return;
        }

        // Update cache.
        static std::map<std::string, std::string> statistics_cache;

        if (statistics_cache == statistics)
        {
            return;
        }

        statistics_cache = statistics;

        // Create JSON.
        Json::Value json_statistics;

        for (const auto& i : statistics)
        {
            Json::Value json_statistics_item;

            json_statistics_item["name"] = i.first;
            json_statistics_item["value"] = i.second;

            json_statistics.append(json_statistics_item);
        }

        // Update UI.
        static std::stringstream stream;

        stream.str("");
        stream << "app.statistics.set(";
        stream << Utils::Convert::to_string(json_statistics);
        stream << ")";

        evaluate_with_timeout(stream.str());
    }

    void UI::set_terrain_layers(const std::set<std::string>& layers)
    {
        if (is_ready() == false)
        {
            return;
        }

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

        evaluate_with_timeout(stream.str());
    }

    void UI::set_water_layers(const std::set<std::string>& layers)
    {
        if (is_ready() == false)
        {
            return;
        }

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

        evaluate_with_timeout(stream.str());
    }

    void UI::init_components()
    {
        cursor = std::make_unique<Cursor>();
        minimap = std::make_unique<Minimap>();
        preview = std::make_unique<Preview>();
        selection_box = std::make_unique<SelectionBox>();
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

    void UI::evaluate_with_timeout(const std::string& js)
    {
        view->EvaluateScript(("setTimeout(() => {" + js + ";}, 0)").c_str());
    }

    void UI::log_write(const std::string& text, const std::string& type, Id id)
    {
        if (is_ready() == false)
        {
            return;
        }

        // Update UI.
        static std::stringstream stream;

        stream.str("");
        stream << "app.log.";
        stream << type;
        stream << "('";
        stream << text;
        stream << "', ";
        stream << id;
        stream << ")";

        evaluate_with_timeout(stream.str());
    }

    void UI::wait_until_ready()
    {
        L_INFO << "Waiting for UI..." << std::endl;

        while (is_ready() == false)
        {
            renderer->Update();
        }
    }
}
