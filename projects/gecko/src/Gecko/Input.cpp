#include "Gecko/Input.hpp"

#include "Gecko/Cameras/Camera.hpp"
#include "Gecko/Containers/Orders.hpp"
#include "Gecko/Containers/Selected.hpp"
#include "Gecko/Exception.hpp"
#include "Gecko/Games/Game.hpp"
#include "Gecko/Key.hpp"
#include "Gecko/Layers/Layer.hpp"
#include "Gecko/Log.hpp"
#include "Gecko/Managers/ConfigurationManager.hpp"
#include "Gecko/Managers/MapManager.hpp"
#include "Gecko/Managers/ObjectManager.hpp"
#include "Gecko/Managers/OrderManager.hpp"
#include "Gecko/Maps/Map.hpp"
#include "Gecko/Objects/Object.hpp"
#include "Gecko/Players/Player.hpp"
#include "Gecko/Statistics.hpp"
#include "Gecko/Containers/Skills.hpp"
#include "Gecko/UI/UI.hpp"
#include "Gecko/UI/Cursor.hpp"
#include "Gecko/UI/SelectionBox.hpp"
#include "Gecko/Utils/Convert.hpp"
#include "Gecko/Utils/Raycast.hpp"
#include "Gecko/Utils/Utils.hpp"

template<> Gecko::Input* Ogre::Singleton<Gecko::Input>::msSingleton = nullptr;

namespace Gecko
{
    bool Input::buttonPressed(const OIS::JoyStickEvent& arg, int button)
    {
        return true;
    }

    bool Input::buttonReleased(const OIS::JoyStickEvent& arg, int button)
    {
        return true;
    }

    bool Input::axisMoved(const OIS::JoyStickEvent& arg, int axis)
    {
        return true;
    }

    bool Input::sliderMoved(const OIS::JoyStickEvent& arg, int index)
    {
        return true;
    }

    bool Input::povMoved(const OIS::JoyStickEvent& arg, int index)
    {
        return true;
    }

    bool Input::vector3Moved(const OIS::JoyStickEvent& arg, int index)
    {
        return true;
    }

    bool Input::keyPressed(const OIS::KeyEvent& arg)
    {
        /*
        // TODO: Implement.
        // Process UI.
        UI::getSingleton().inject_key_press(Convert::to_char(
            arg.key, is_key_pressed(OIS::KeyCode::KC_LSHIFT) || is_key_pressed(OIS::KeyCode::KC_RSHIFT)
        ));
        */

        // Call command based on pressed key.
        auto assign_to_group = [](std::uint8_t group_number)
        {
            Game::getSingleton().get_active_player()->get_selected()->create_group(group_number);
        };

        auto select_group = [](std::uint8_t group_number)
        {
            Game::getSingleton().get_active_player()->get_selected()->select_group(group_number);
        };

        auto switch_player = [](std::uint8_t player_id)
        {
            Game::getSingleton().set_active_player_id(player_id);
        };

        static const std::map<Command::Value, std::function<void()>> commands =
        {
            // TODO: Create method, because active player can change.
            { Command::Value::Assign_To_Group_0, std::bind(assign_to_group, 0) },
            { Command::Value::Assign_To_Group_1, std::bind(assign_to_group, 1) },
            { Command::Value::Assign_To_Group_2, std::bind(assign_to_group, 2) },
            { Command::Value::Assign_To_Group_3, std::bind(assign_to_group, 3) },
            { Command::Value::Assign_To_Group_4, std::bind(assign_to_group, 4) },
            { Command::Value::Assign_To_Group_5, std::bind(assign_to_group, 5) },
            { Command::Value::Assign_To_Group_6, std::bind(assign_to_group, 6) },
            { Command::Value::Assign_To_Group_7, std::bind(assign_to_group, 7) },
            { Command::Value::Assign_To_Group_8, std::bind(assign_to_group, 8) },
            { Command::Value::Assign_To_Group_9, std::bind(assign_to_group, 9) },

            // TODO: Restore time control.
            // { Command::Value::Game_Faster, std::bind(&Game::faster, Game::getSingletonPtr()) },
            // { Command::Value::Game_Pause, std::bind(&Game::toggle_pause, Game::getSingletonPtr()) },
            // { Command::Value::Game_Slower, std::bind(&Game::slower, Game::getSingletonPtr()) },

            // TODO: Create method, because active player can change.
            { Command::Value::Select_Group_0, std::bind(select_group, 0) },
            { Command::Value::Select_Group_1, std::bind(select_group, 1) },
            { Command::Value::Select_Group_2, std::bind(select_group, 2) },
            { Command::Value::Select_Group_3, std::bind(select_group, 3) },
            { Command::Value::Select_Group_4, std::bind(select_group, 4) },
            { Command::Value::Select_Group_5, std::bind(select_group, 5) },
            { Command::Value::Select_Group_6, std::bind(select_group, 6) },
            { Command::Value::Select_Group_7, std::bind(select_group, 7) },
            { Command::Value::Select_Group_8, std::bind(select_group, 8) },
            { Command::Value::Select_Group_9, std::bind(select_group, 9) },

            { Command::Value::Switch_Player_1, std::bind(switch_player, 1) },
            { Command::Value::Switch_Player_2, std::bind(switch_player, 2) },
            { Command::Value::Switch_Player_3, std::bind(switch_player, 3) },
            { Command::Value::Switch_Player_4, std::bind(switch_player, 4) },
            { Command::Value::Switch_Player_5, std::bind(switch_player, 5) },
            { Command::Value::Switch_Player_6, std::bind(switch_player, 6) },
            { Command::Value::Switch_Player_7, std::bind(switch_player, 7) },
            { Command::Value::Switch_Player_8, std::bind(switch_player, 8) },

            { Command::Value::UI_Show_Menu, std::bind(&UI::show_menu, UI::getSingletonPtr()) },
            { Command::Value::UI_Toggle_Floating_Description, std::bind(&UI::toggle_floating_description, UI::getSingletonPtr()) },

            { Command::Value::Quit, std::bind(&Game::shutdown, Game::getSingletonPtr()) }
        };

        for (const auto& [command, function] : commands)
        {
            if (is_key_pressed(command))
            {
                function();

                break;
            }
        }

        return true;
    }

    bool Input::keyReleased(const OIS::KeyEvent& arg)
    {
        Statistics::getSingleton().add("Pressed keys", 1.0f);

        /*
        // Process UI.
        UI::getSingleton().inject_key_press(Convert::to_char(
        arg.key, is_key_pressed(OIS::KeyCode::KC_LSHIFT) || is_key_pressed(OIS::KeyCode::KC_RSHIFT)
        ));
        */

        return true;
    }

    bool Input::mouseMoved(const OIS::MouseEvent& arg)
    {
        auto x = static_cast<float>(arg.state.X.rel);
        auto y = static_cast<float>(arg.state.Y.rel);

        UI::getSingleton().inject_mouse_move(arg.state.X.abs, arg.state.Y.abs);
        UI::getSingleton().get_cursor().set_visible(false);

        auto camera = MapManager::getSingleton().begin()->second->get_camera(Settings::Camera::MainName);
        camera->get_camera_node()->translate(Ogre::Vector3::NEGATIVE_UNIT_Z * static_cast<float>(arg.state.Z.rel) * 0.1f);

        if (UI::getSingleton().is_mouse_inside(arg.state.X.abs, arg.state.Y.abs))
        {
            UI::getSingleton().get_selection_box().set_visible(false);
        }
        else if (is_mouse_button_pressed(OIS::MouseButtonID::MB_Left))
        {
            auto& selection_box = UI::getSingleton().get_selection_box();

            selection_box.set_end(Utils::Convert::to_screen_coordinates(arg));
            selection_box.update();

            UI::getSingleton().get_cursor().set_visible(true);
        }
        else if (is_mouse_button_pressed(OIS::MouseButtonID::MB_Middle))
        {
            camera->yaw(Ogre::Degree(x * mouse_sensitivity.x));
            camera->pitch(Ogre::Degree(y * mouse_sensitivity.y));

            UI::getSingleton().get_cursor().set_visible(true);
        }
        else
        {
            auto object_cast = Utils::Raycast::to_object(arg);

            if (object_cast)
            {
                UI::getSingleton().set_hovered_object_id(object_cast->first);
                UI::getSingleton().get_cursor().set_visible(true);
            }
            else
            {
                UI::getSingleton().set_hovered_object_id(Id::Empty);

                auto layer_cast = Utils::Raycast::to_layer(arg);

                if (layer_cast)
                {
                    UI::getSingleton().get_cursor().update(layer_cast.value());
                    UI::getSingleton().get_cursor().set_visible(true);
                }
            }
        }

        Statistics::getSingleton().add("Mouse distance", std::abs(x) + std::abs(y));

        return true;
    }

    bool Input::mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id)
    {
        if (UI::getSingleton().is_mouse_inside(arg.state.X.abs, arg.state.Y.abs))
        {
            UI::getSingleton().inject_mouse_press(arg.state.X.abs, arg.state.Y.abs, id);
        }
        else if (id == OIS::MouseButtonID::MB_Left)
        {
            UI::getSingleton().get_selection_box().set_start(Utils::Convert::to_screen_coordinates(arg));
        }

        return true;
    }

    bool Input::mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id)
    {
        if (UI::getSingleton().is_mouse_inside(arg.state.X.abs, arg.state.Y.abs))
        {
            UI::getSingleton().inject_mouse_release(arg.state.X.abs, arg.state.Y.abs, id);
        }
        else if (id == OIS::MouseButtonID::MB_Left)
        {
            handle_left_mouse_button(arg);
        }
        else if (id == OIS::MouseButtonID::MB_Right)
        {
            handle_right_mouse_button(arg);
        }

        Statistics::getSingleton().add("Mouse clicks", 1.0f);

        return true;
    }

    Input::Input(const std::shared_ptr<Configuration>& configuration) :
        m_configuration(configuration)
    {
    }

    void Input::init()
    {
        L_TRACE << "Input::Input()";

        auto commands_configuration = m_configuration->get_child("commands");

        for (auto i = commands_configuration->begin(); i != commands_configuration->end(); i++)
        {
            for (auto j = i->begin(); j != i->end(); ++j)
            {
                commands[Command::from_string(i.key().asString())].emplace(Key::from_string(j->asString()));
            }
        }

        OIS::ParamList param_list;

        param_list.emplace(OIS::ParamList::value_type("WINDOW", Ogre::StringConverter::toString(m_render_window_handle)));

#if defined OIS_WIN32_PLATFORM
        param_list.emplace(OIS::ParamList::value_type("w32_keyboard", "DISCL_FOREGROUND"));
        param_list.emplace(OIS::ParamList::value_type("w32_keyboard", "DISCL_NONEXCLUSIVE"));
        param_list.emplace(OIS::ParamList::value_type("w32_mouse", "DISCL_FOREGROUND"));
        param_list.emplace(OIS::ParamList::value_type("w32_mouse", "DISCL_NONEXCLUSIVE"));
#elif
        param_list.emplace(OIS::ParamList::value_type("XAutoRepeatOn", "true"));
        param_list.emplace(OIS::ParamList::value_type("x11_keyboard_grab", "false"));
        param_list.emplace(OIS::ParamList::value_type("x11_mouse_grab", "false"));
        param_list.emplace(OIS::ParamList::value_type("x11_mouse_hide", "false"));
#endif

        input_manager = OIS::InputManager::createInputSystem(param_list);

        if (input_manager->getNumberOfDevices(OIS::Type::OISJoyStick) > 0)
        {
            joystick = dynamic_cast<OIS::JoyStick*>(input_manager->createInputObject(OIS::Type::OISJoyStick, true));
            joystick->setEventCallback(this);
        }

        if (input_manager->getNumberOfDevices(OIS::Type::OISKeyboard) > 0)
        {
            keyboard = static_cast<OIS::Keyboard*>(input_manager->createInputObject(OIS::OISKeyboard, true));
            keyboard->setEventCallback(this);
        }

        if (input_manager->getNumberOfDevices(OIS::Type::OISMouse) > 0)
        {
            mouse = static_cast<OIS::Mouse*>(input_manager->createInputObject(OIS::OISMouse, true));
            mouse->setEventCallback(this);
        }
    }

    void Input::deinit()
    {
        if (joystick)
        {
            input_manager->destroyInputObject(joystick);
        }

        if (keyboard)
        {
            input_manager->destroyInputObject(keyboard);
        }

        if (mouse)
        {
            input_manager->destroyInputObject(mouse);
        }

        OIS::InputManager::destroyInputSystem(input_manager);
    }

    void Input::update(float time)
    {
        if (joystick)
        {
            joystick->capture();
        }

        if (keyboard)
        {
            keyboard->capture();
        }

        if (mouse)
        {
            mouse->capture();
        }
    }

    bool Input::is_key_pressed(Command::Value command) const
    {
        auto keys = commands.find(command);

        if (keys == commands.end())
        {
            return false;
        }

        bool pressed = true;

        for (const auto& key : keys->second)
        {
            if (is_key_pressed(key) == false)
            {
                pressed = false;

                break;
            }
        }

        return pressed;
    }

    bool Input::is_mouse_button_pressed(OIS::MouseButtonID mouse_button) const
    {
        return mouse->getMouseState().buttonDown(mouse_button);
    }

    void Input::set_mouse_sensitivity(const Ogre::Vector2& sensivity)
    {
        mouse_sensitivity = sensivity;
    }

    void Input::set_render_window_handle(unsigned long render_window_handle)
    {
        m_render_window_handle = render_window_handle;
    }

    void Input::set_window_size(int width, int height)
    {
        if (mouse)
        {
            const auto& mouse_state = mouse->getMouseState();

            mouse_state.width = width;
            mouse_state.height = height;
        }
    }

    void Input::process_attack_order(const OIS::MouseEvent& arg)
    {
        if (Game::getSingleton().get_active_player()->get_selected()->size())
        {
            auto object_result = Utils::Raycast::to_object(arg);
            auto terrain_result = Utils::Raycast::to_layer(arg);

            for (const auto& object_id : *(Game::getSingleton().get_active_player()->get_selected()))
            {
                auto object = ObjectManager::getSingleton().get(object_id);

                if (object == nullptr)
                {
                    continue;
                }

                if (is_key_pressed(Command::Value::Multiple_Order) == false)
                {
                    object->get_orders()->remove_all_orders();
                }

                if (object_result && object_result->first.is_valid())
                {
                    auto order = OrderManager::getSingleton().order_attack(Id::Empty, object_id, object_result->first);

                    if (order == nullptr)
                    {
                        throw Exception("Failed to create order.");
                    }

                    object->get_orders()->add_last(order->get_id());
                }
                else
                {
                    auto order = OrderManager::getSingleton().order_attack(Id::Empty, object_id, terrain_result->second);

                    if (order == nullptr)
                    {
                        throw Exception("Failed to create order.");
                    }

                    object->get_orders()->add_last(order->get_id());
                }
            }
        }
    }

    void Input::process_create_order(const OIS::MouseEvent& arg)
    {
        auto terrain_result = Utils::Raycast::to_layer(arg);

        if (terrain_result)
        {
            OrderManager::getSingleton().order_create(
                Id::Empty, Id::Empty,
                UI::getSingleton().get_configuration_name(),
                terrain_result->second,
                Game::getSingleton().get_active_player_id()
            );
        }
    }

    void Input::process_follow_order(const OIS::MouseEvent& arg)
    {
        if (Game::getSingleton().get_active_player()->get_selected()->size() == 0)
        {
            return;
        }

        auto object_result = Utils::Raycast::to_object(arg);

        for (const auto& object_id : *(Game::getSingleton().get_active_player()->get_selected()))
        {
            auto object = ObjectManager::getSingleton().get(object_id);

            if (object == nullptr)
            {
                continue;
            }

            if (is_key_pressed(Command::Value::Multiple_Order) == false)
            {
                object->get_orders()->remove_all_orders();
            }

            if (object_result && object_result->first.is_valid())
            {
                auto order = OrderManager::getSingleton().order_follow(Id::Empty, object_id, object_result->first);

                object->get_orders()->add_last(order->get_id());
            }
        }
    }

    void Input::process_guard_order(const OIS::MouseEvent& arg)
    {
        if (Game::getSingleton().get_active_player()->get_selected()->size() == 0)
        {
            return;
        }

        auto object_result = Utils::Raycast::to_object(arg);
        auto terrain_result = Utils::Raycast::to_layer(arg);

        for (const auto& object_id : *(Game::getSingleton().get_active_player()->get_selected()))
        {
            auto object = ObjectManager::getSingleton().get(object_id);

            if (object == nullptr)
            {
                continue;
            }

            if (is_key_pressed(Command::Value::Multiple_Order) == false)
            {
                object->get_orders()->remove_all_orders();
            }

            if (object_result && object_result->first.is_valid())
            {
                auto order = OrderManager::getSingleton().order_guard(Id::Empty, object_id, object_result->first);

                if (order == nullptr)
                {
                    throw Exception("Failed to create order.");
                }

                object->get_orders()->add_last(order->get_id());
            }
            else
            {
                auto order = OrderManager::getSingleton().order_guard(Id::Empty, object_id, terrain_result->second);

                if (order == nullptr)
                {
                    throw Exception("Failed to create order.");
                }

                object->get_orders()->add_last(order->get_id());
            }
        }
    }

    void Input::process_idle_order(const OIS::MouseEvent& arg)
    {
        /*
        if (id == OIS::MouseButtonID::MB_Left)
        {
            auto result = Utils::Raycast::cast_ray(arg, Object::QueryFlags::QF_OBJECT);
            auto object = ObjectManager::getSingleton().get(result.first);

            if (object)
            {
                Game::getSingleton().get_active_player()->select(object->get_id(), is_key_pressed(Command::Value::Multiple_Select));
            }
            else
            {
                Game::getSingleton().get_active_player()->select(0, is_key_pressed(Command::Value::Multiple_Select));
            }
        }
        else if (id == OIS::MouseButtonID::MB_Right)
        {
            if (Game::getSingleton().get_active_player()->get_selected()->size())
            {
                for (const auto& selected_object_id : Game::getSingleton().get_active_player()->get_selected())
                {
                    auto selected_object = ObjectManager::getSingleton().get(selected_object_id);

                    if (selected_object)
                    {
                        if (selected_object->get_default_order() == order_type::Value::Move)
                        {
                            auto result = Utils::Raycast::cast_ray(arg, Object::QueryFlags::QF_LAYER);

                            if (is_key_pressed(Command::Value::Multiple_Order) == false)
                            {
                                OrderManager::getSingleton().stop(selected_object_id);
                            }

                            OrderManager::getSingleton().move(selected_object_id, result.second);
                        }
                        else if (selected_object->get_default_order() == order_type::Value::Rally)
                        {
                            auto result = Utils::Raycast::cast_ray(arg, Object::QueryFlags::QF_LAYER);

                            OrderManager::getSingleton().rally(selected_object_id, result.second);
                        }
                    }
                }
            }
        }
        */
    }

    void Input::process_move_order(const OIS::MouseEvent& arg)
    {
        auto selected = Game::getSingleton().get_active_player()->get_selected();

        if (selected->size() == 0)
        {
            return;
        }

        auto layer_result = Utils::Raycast::to_layer(arg);

        if (layer_result.has_value() == false)
        {
            return;
        }

        for (const auto& object_id : *(selected))
        {
            auto object = ObjectManager::getSingleton().get(object_id);

            if (object == nullptr)
            {
                continue;
            }

            if (is_key_pressed(Command::Value::Multiple_Order) == false)
            {
                object->get_orders()->remove_all_orders();
            }

            auto order = OrderManager::getSingleton().order_move(Id::Empty, object_id, layer_result->second);

            if (order == nullptr)
            {
                throw Exception("Failed to create order.");
            }

            object->get_orders()->add_last(order->get_id());
        }
    }

    void Input::process_patrol_order(const OIS::MouseEvent& arg)
    {
        if (Game::getSingleton().get_active_player()->get_selected()->size() == 0)
        {
            return;
        }

        auto object_result = Utils::Raycast::to_object(arg);
        auto terrain_result = Utils::Raycast::to_layer(arg);

        for (const auto& object_id : *(Game::getSingleton().get_active_player()->get_selected()))
        {
            auto object = ObjectManager::getSingleton().get(object_id);

            if (object == nullptr)
            {
                continue;
            }

            if (is_key_pressed(Command::Value::Multiple_Order) == false)
            {
                object->get_orders()->remove_all_orders();
            }

            if (object_result && object_result->first.is_valid())
            {
                auto order = OrderManager::getSingleton().order_patrol(Id::Empty, object_id, object_result->first);

                if (order == nullptr)
                {
                    throw Exception("Failed to create order.");
                }

                object->get_orders()->add_last(order->get_id());
            }
            else
            {
                auto order = OrderManager::getSingleton().order_patrol(Id::Empty, object_id, terrain_result->second);

                if (order == nullptr)
                {
                    throw Exception("Failed to create order.");
                }

                object->get_orders()->add_last(order->get_id());
            };
        }
    }

    void Input::process_rally_order(const OIS::MouseEvent& arg)
    {
        if (Game::getSingleton().get_active_player()->get_selected()->size() == 0)
        {
            return;
        }

        // TODO: Check if at least one cast is correct.
        auto object_result = Utils::Raycast::to_object(arg);
        auto terrain_result = Utils::Raycast::to_layer(arg);

        for (const auto& object_id : *(Game::getSingleton().get_active_player()->get_selected()))
        {
            auto object = ObjectManager::getSingleton().get(object_id);

            if (object == nullptr)
            {
                continue;
            }

            if (is_key_pressed(Command::Value::Multiple_Order) == false)
            {
                object->get_orders()->remove_all_orders();
            }

            if (object_result && object_result->first.is_valid())
            {
                auto order = OrderManager::getSingleton().order_rally(Id::Empty, object_id, object_result->first);

                if (order == nullptr)
                {
                    throw Exception("Failed to create order.");
                }

                object->get_orders()->add_last(order->get_id());
            }
            else
            {
                auto order = OrderManager::getSingleton().order_rally(Id::Empty, object_id, terrain_result->second);

                if (order == nullptr)
                {
                    throw Exception("Failed to create order.");
                }

                object->get_orders()->add_last(order->get_id());
            };
        }
    }

    void Input::process_skill(const OIS::MouseEvent& arg, const std::string& skill_name)
    {
        if (Game::getSingleton().get_active_player()->get_selected()->size() == 0)
        {
            return;
        }

        // TODO: Check if at least one cast is correct.
        auto object_result = Utils::Raycast::to_object(arg);
        auto terrain_result = Utils::Raycast::to_layer(arg);

        for (const auto& object_id : *(Game::getSingleton().get_active_player()->get_selected()))
        {
            auto object = ObjectManager::getSingleton().get(object_id);

            if (object == nullptr)
            {
                continue;
            }

            if (object_result && object_result->first.is_valid())
            {
                object->get_skills()->activate(skill_name, object_result->first);
            }
            else
            {
                object->get_skills()->activate(skill_name, terrain_result->second);
            }
        }
    }

    void Input::handle_left_mouse_button(const OIS::MouseEvent& arg)
    {
        auto& ui = UI::getSingleton();
        auto& selection_box = ui.get_selection_box();

        selection_box.set_end(Utils::Convert::to_screen_coordinates(arg));
        selection_box.set_visible(false);

        const auto& configuration_name = ui.get_configuration_name();
        auto possible_order_name = ui.get_order_name();
        const auto& skill_name = ui.get_skill_name();

        // TODO: Do something with "none".
        if (configuration_name != "none")
        {
            auto layer_cast = Utils::Raycast::to_layer(arg);

            if (layer_cast.has_value() == false)
            {
                ui.log_error("Failed to create object '" + configuration_name + "'.");
                ui.reset();

                return;
            }

            auto object = ObjectManager::getSingleton().create(configuration_name);

            if (object == nullptr)
            {
                ui.log_error("Failed to create object '" + configuration_name + "'.");
                ui.reset();

                return;
            }

            object->set_owner(layer_cast->first->get_owner());
            object->init();
            object->set_player_id(Game::getSingleton().get_active_player_id());
            object->set_position(layer_cast->first->get_position(layer_cast->second));

            UI::getSingleton().log_error("Object " + configuration_name + " created.");

            if (is_key_pressed(Command::Value::Multiple_Order) == false)
            {
                UI::getSingleton().reset();
            }
        }
        else if (possible_order_name != order_type::Value::None)
        {
            const std::map<order_type::Value, std::function<void(const OIS::MouseEvent&)>> handlers =
            {
                { order_type::Value::Attack, std::bind(&Input::process_attack_order, this, std::placeholders::_1) },
                { order_type::Value::Follow, std::bind(&Input::process_follow_order, this, std::placeholders::_1) },
                { order_type::Value::Guard, std::bind(&Input::process_guard_order, this, std::placeholders::_1) },
                { order_type::Value::Move, std::bind(&Input::process_move_order, this, std::placeholders::_1) },
                { order_type::Value::Patrol, std::bind(&Input::process_patrol_order, this, std::placeholders::_1) },
                { order_type::Value::Rally, std::bind(&Input::process_rally_order, this, std::placeholders::_1) }
            };

            auto handler = handlers.find(possible_order_name);

            if (handler != handlers.end())
            {
                handler->second(arg);
            }
            else
            {
                L_WARNING << "Could not find handler for order '" << order_type::to_string(possible_order_name) << "'.";
            }

            if (is_key_pressed(Command::Value::Multiple_Order) == false)
            {
                UI::getSingleton().reset();
            }
        }
        // TODO: Do something with "none".
        else if (skill_name != "none")
        {
            process_skill(arg, skill_name);
        }
        else
        {
            if (selection_box.is_valid())
            {
                auto object_cast = Utils::Raycast::to_objects(selection_box.get_start(), selection_box.get_end());
                auto player = Game::getSingleton().get_active_player();

                if (player)
                {
                    player->get_selected()->select(object_cast, is_key_pressed(Command::Value::Multiple_Select));
                }
            }
            else
            {
                auto object_cast = Utils::Raycast::to_object(arg);
                auto player = Game::getSingleton().get_active_player();

                if (player)
                {
                    auto selected = player->get_selected();

                    if (object_cast)
                    {
                        selected->select(object_cast->first, is_key_pressed(Command::Value::Multiple_Select));
                    }
                    else
                    {
                        selected->select(Id::Empty, is_key_pressed(Command::Value::Multiple_Select));
                    }
                }
            }
        }
    }

    void Input::handle_right_mouse_button(const OIS::MouseEvent& arg)
    {
        auto player = Game::getSingleton().get_active_player();

        if (player == nullptr)
        {
            return;
        }

        auto selected = player->get_selected();

        if (selected->empty())
        {
            return;
        }

        auto object_cast = Utils::Raycast::to_object(arg);

        if (object_cast.has_value())
        {
            auto object = ObjectManager::getSingleton().get(object_cast->first);

            if (object == nullptr)
            {
                return;
            }

            if (Utils::is_friendly(*object, *player))
            {
                process_guard_order(arg);
            }
            else
            {
                process_attack_order(arg);
            }
        }
        else
        {
            auto layer_cast = Utils::Raycast::to_layer(arg);

            if (layer_cast)
            {
                process_move_order(arg);
            }
        }
    }
}
