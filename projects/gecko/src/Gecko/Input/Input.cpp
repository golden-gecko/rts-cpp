#include "Gecko/Input/Input.hpp"

#include "Gecko/Cameras/Camera.hpp"
#include "Gecko/Configuration.hpp"
#include "Gecko/Containers/Orders.hpp"
#include "Gecko/Containers/Selection.hpp"
#include "Gecko/Containers/Skills.hpp"
#include "Gecko/Diplomacy.hpp"
#include "Gecko/Exception.hpp"
#include "Gecko/Games/Game.hpp"
#include "Gecko/Input/Key.hpp"
#include "Gecko/Input/Strategies/AttackOrderStrategy.hpp"
#include "Gecko/Input/Strategies/FollowOrderStrategy.hpp"
#include "Gecko/Input/Strategies/GuardOrderStrategy.hpp"
#include "Gecko/Input/Strategies/MoveOrderStrategy.hpp"
#include "Gecko/Input/Strategies/PatrolOrderStrategy.hpp"
#include "Gecko/Input/Strategies/RallyOrderStrategy.hpp"
#include "Gecko/Layers/Layer.hpp"
#include "Gecko/Log.hpp"
#include "Gecko/Managers/ConfigurationManager.hpp"
#include "Gecko/Managers/MapManager.hpp"
#include "Gecko/Managers/ObjectManager.hpp"
#include "Gecko/Managers/OrderManager.hpp"
#include "Gecko/Maps/Map.hpp"
#include "Gecko/Objects/Object.hpp"
#include "Gecko/Players/Player.hpp"
#include "Gecko/UI/Widgets/Cursor.hpp"
#include "Gecko/UI/Widgets/Preview.hpp"
#include "Gecko/UI/Widgets/Configurations.hpp"
#include "Gecko/UI/Widgets/Orders.hpp"
#include "Gecko/UI/Widgets/SelectionBox.hpp"
#include "Gecko/UI/Widgets/Skills.hpp"
#include "Gecko/UI/UI.hpp"
#include "Gecko/Utils/Convert.hpp"
#include "Gecko/Utils/Raycast.hpp"
#include "Gecko/Utils/Utils.hpp"

Gecko::Input* Ogre::Singleton<Gecko::Input>::msSingleton = nullptr;

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
        bool result = UI::getSingleton().inject_key_press(Utils::Convert::to_rmlui_key(arg.key));

        // Pass only ASCII characters.
        if (arg.text >= Settings::Input::TextMin && arg.text <= Settings::Input::TextMax)
        {
            result &= UI::getSingleton().inject_text(arg.text);
        }

        if (result == false)
        {
            return true;
        }

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
            Game::getSingleton().set_active_player_id(Id(player_id));
        };

        static const std::map<Command::Value, std::function<void()>> commands =
        {
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

            { Command::Value::Game_Faster, std::bind(&Game::faster, Game::getSingletonPtr()) },
            { Command::Value::Game_Pause, std::bind(&Game::pause, Game::getSingletonPtr()) },
            { Command::Value::Game_Slower, std::bind(&Game::slower, Game::getSingletonPtr()) },

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

            { Command::Value::Quit, std::bind(&Game::stop, Game::getSingletonPtr()) }
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
        if (UI::getSingleton().inject_key_release(Utils::Convert::to_rmlui_key(arg.key)) == false)
        {
            return true;
        }

        return true;
    }

    bool Input::mouseMoved(const OIS::MouseEvent& arg)
    {
        int x_abs = static_cast<float>(arg.state.X.abs);
        int y_abs = static_cast<float>(arg.state.Y.abs);

        int x_rel = static_cast<float>(arg.state.X.rel);
        int y_rel = static_cast<float>(arg.state.Y.rel);
        int z_rel = static_cast<float>(arg.state.Z.rel);

        if (UI::getSingleton().inject_mouse_move(x_abs, y_abs, z_rel))
        {
            return true;
        }

        // UI::getSingleton().get_cursor().set_visible(false);

        if (MapPtr map = Game::getSingleton().get_active_map())
        {
            if (CameraPtr camera = map->get_camera(Settings::Camera::Main))
            {
                if (UI::getSingleton().is_mouse_inside(x_abs, y_abs))
                {
                    UI::getSingleton().get_component<SelectionBoxWidget>()->set_visible(false);
                }
                else if (is_mouse_button_pressed(OIS::MouseButtonID::MB_Left))
                {
                    auto selection_box = UI::getSingleton().get_component<SelectionBoxWidget>();

                    selection_box->set_end(Utils::Convert::to_screen_coordinates(arg));
                    selection_box->update();

                    // UI::getSingleton().get_cursor().set_visible(true);
                }
                else if (is_mouse_button_pressed(OIS::MouseButtonID::MB_Middle))
                {
                    camera->rotate(Ogre::Degree(x_rel * m_mouse_sensitivity.x), Ogre::Degree(y_rel * m_mouse_sensitivity.y));

                    // UI::getSingleton().get_cursor().set_visible(true);
                }
                else
                {
                    auto object_cast = Utils::Raycast::to_object(Game::getSingleton().get_map_scene(), arg);

                    if (object_cast)
                    {
                        UI::getSingleton().set_hovered_object_id(object_cast->first);
                        // UI::getSingleton().get_cursor().set_visible(true);
                    }
                    else
                    {
                        UI::getSingleton().set_hovered_object_id(Id::Empty);
                        // UI::getSingleton().get_preview()->get_camera()->set_target_id(Id::Empty);

                        auto layer_cast = Utils::Raycast::to_layer(Game::getSingleton().get_map_scene(), arg);

                        if (layer_cast)
                        {
                            // UI::getSingleton().get_cursor().update(layer_cast.value());
                            // UI::getSingleton().get_cursor().set_visible(true);
                        }
                    }
                }
            }
        }

        return true;
    }

    bool Input::mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id)
    {
        if (UI::getSingleton().is_mouse_inside(arg.state.X.abs, arg.state.Y.abs))
        {
            return UI::getSingleton().inject_mouse_press(Utils::Convert::to_rmlui_button(id));
        }

        if (id == OIS::MouseButtonID::MB_Left)
        {
            UI::getSingleton().get_component<SelectionBoxWidget>()->set_start(Utils::Convert::to_screen_coordinates(arg));
        }

        return true;
    }

    bool Input::mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id)
    {
        if (UI::getSingleton().is_mouse_inside(arg.state.X.abs, arg.state.Y.abs))
        {
            return UI::getSingleton().inject_mouse_release(Utils::Convert::to_rmlui_button(id));
        }

        if (id == OIS::MouseButtonID::MB_Left)
        {
            handle_left_mouse_button(arg);
        }
        else if (id == OIS::MouseButtonID::MB_Right)
        {
            handle_right_mouse_button(arg);
        }

        return true;
    }

    Input::Input(const ConfigurationPtr& configuration, std::uint64_t render_window_handle) :
        m_configuration(configuration),
        m_render_window_handle(render_window_handle)
    {
    }

    void Input::init()
    {
        ConfigurationPtr commands_configuration = m_configuration->get_child("commands");

        for (auto i = commands_configuration->begin(); i != commands_configuration->end(); i++)
        {
            for (auto j = i->begin(); j != i->end(); ++j)
            {
                m_commands[Command::from_string(i.key().asString())].emplace(Key::from_string(j->asString()));
            }
        }

        OIS::ParamList param_list =
        {
            OIS::ParamList::value_type("WINDOW", std::to_string(m_render_window_handle)),
            OIS::ParamList::value_type("w32_keyboard", "DISCL_FOREGROUND"),
            OIS::ParamList::value_type("w32_keyboard", "DISCL_NONEXCLUSIVE"),
            OIS::ParamList::value_type("w32_mouse", "DISCL_FOREGROUND"),
            OIS::ParamList::value_type("w32_mouse", "DISCL_NONEXCLUSIVE")
        };

        m_input_manager = OIS::InputManager::createInputSystem(param_list);

        if (m_input_manager->getNumberOfDevices(OIS::Type::OISJoyStick) > 0)
        {
            m_joystick = dynamic_cast<OIS::JoyStick*>(m_input_manager->createInputObject(OIS::Type::OISJoyStick, true));
            m_joystick->setEventCallback(this);
        }

        if (m_input_manager->getNumberOfDevices(OIS::Type::OISKeyboard) > 0)
        {
            m_keyboard = static_cast<OIS::Keyboard*>(m_input_manager->createInputObject(OIS::OISKeyboard, true));
            m_keyboard->setEventCallback(this);
        }

        if (m_input_manager->getNumberOfDevices(OIS::Type::OISMouse) > 0)
        {
            m_mouse = static_cast<OIS::Mouse*>(m_input_manager->createInputObject(OIS::OISMouse, true));
            m_mouse->setEventCallback(this);
        }
    }

    void Input::deinit()
    {
        if (m_joystick)
        {
            m_input_manager->destroyInputObject(m_joystick);
        }

        if (m_keyboard)
        {
            m_input_manager->destroyInputObject(m_keyboard);
        }

        if (m_mouse)
        {
            m_input_manager->destroyInputObject(m_mouse);
        }

        OIS::InputManager::destroyInputSystem(m_input_manager);
    }

    void Input::update(float time)
    {
        if (m_joystick)
        {
            m_joystick->capture();
        }

        if (m_keyboard)
        {
            m_keyboard->capture();
        }

        if (m_mouse)
        {
            m_mouse->capture();
        }
    }

    bool Input::is_key_pressed(Command::Value command) const
    {
        auto keys = m_commands.find(command);

        if (keys == m_commands.end())
        {
            return false;
        }

        return std::all_of(keys->second.begin(), keys->second.end(), [this](OIS::KeyCode key) {
            return is_key_pressed(key);
        });
    }

    bool Input::is_key_pressed(OIS::KeyCode key_code) const
    {
        return m_keyboard ? m_keyboard->isKeyDown(key_code) : false;
    }

    bool Input::is_mouse_button_pressed(OIS::MouseButtonID mouse_button) const
    {
        return m_mouse ? m_mouse->getMouseState().buttonDown(mouse_button) : false;
    }

    void Input::set_mouse_sensitivity(const Ogre::Vector2& mouse_sensitivity)
    {
        m_mouse_sensitivity = mouse_sensitivity;
    }

    void Input::set_window_size(int width, int height)
    {
        if (m_mouse)
        {
            const OIS::MouseState& mouse_state = m_mouse->getMouseState();

            mouse_state.width = width;
            mouse_state.height = height;
        }
    }

    void Input::handle_left_mouse_button(const OIS::MouseEvent& arg)
    {
        auto configurations = UI::getSingleton().get_component<ConfigurationsWidget>();
        auto orders = UI::getSingleton().get_component<OrdersWidget>();
        auto selection_box = UI::getSingleton().get_component<SelectionBoxWidget>();
        auto skills = UI::getSingleton().get_component<SkillsWidget>();

        const std::string& selected_configuration = configurations->get_selected();
        const std::string& selected_order = orders->get_selected();
        const std::string& selected_skill = skills->get_selected();

        if (selected_configuration.empty() == false)
        {
            handle_configuration(arg, selected_configuration);
        }
        else if (selected_order.empty() == false)
        {
            handle_order(arg, selected_order);
        }
        else if (selected_skill.empty() == false)
        {
            handle_skill(arg, selected_skill);
        }
        else
        {
            handle_selection(arg);
        }
    }

    void Input::handle_right_mouse_button(const OIS::MouseEvent& arg)
    {
        PlayerPtr player = Game::getSingleton().get_active_player();

        if (player == nullptr)
        {
            return;
        }

        if (ObjectRaycastResults object_cast = Utils::Raycast::to_object(Game::getSingleton().get_map_scene(), arg))
        {
            if (ObjectPtr object = ObjectManager::getSingleton().get(object_cast->first))
            {
                if (Diplomacy::getSingleton().get(object, player) == Diplomacy::State::Ally)
                {
                    GuardOrderStrategy strategy;
                    strategy.execute(arg);
                }
                else if (Diplomacy::getSingleton().get(object, player) == Diplomacy::State::Enemy)
                {
                    AttackOrderStrategy strategy;
                    strategy.execute(arg);
                }
            }
        }
        else if (LayerRaycastResults layer_cast = Utils::Raycast::to_layer(Game::getSingleton().get_map_scene(), arg))
        {
            MoveOrderStrategy strategy;
            strategy.execute(arg);
        }
    }

    void Input::handle_configuration(const OIS::MouseEvent& arg, const std::string& selected_configuration)
    {
        LayerRaycastResults layer_cast = Utils::Raycast::to_layer(Game::getSingleton().get_map_scene(), arg);

        if (layer_cast.has_value() == false)
        {
            UI::getSingleton().log_error("Failed to create object '" + selected_configuration + "'.");
            UI::getSingleton().reset();

            return;
        }

        ObjectPtr object = ObjectManager::getSingleton().create(selected_configuration);

        if (object == nullptr)
        {
            UI::getSingleton().log_error("Failed to create object '" + selected_configuration + "'.");
            UI::getSingleton().reset();

            return;
        }

        object->set_owner(layer_cast->first->get_owner());
        object->init();
        object->set_player_id(Game::getSingleton().get_active_player_id());
        object->set_position(layer_cast->first->get_position(layer_cast->second));

        UI::getSingleton().log_error("Object " + selected_configuration + " created.");

        if (is_key_pressed(Command::Value::Multiple_Order) == false)
        {
            UI::getSingleton().reset();
        }
    }

    void Input::handle_order(const OIS::MouseEvent& arg, const std::string& selected_order)
    {
        static const std::map<order_type::Value, std::shared_ptr<OrderStrategy>> handlers =
        {
            { order_type::Value::Attack, std::make_shared<AttackOrderStrategy>() },
            { order_type::Value::Follow, std::make_shared<FollowOrderStrategy>() },
            { order_type::Value::Guard, std::make_shared<GuardOrderStrategy>() },
            { order_type::Value::Move, std::make_shared<MoveOrderStrategy>() },
            { order_type::Value::Patrol, std::make_shared<PatrolOrderStrategy>() },
            { order_type::Value::Rally, std::make_shared<RallyOrderStrategy>() }
        };

        auto handler = handlers.find(order_type::from_string(selected_order));

        if (handler != handlers.end())
        {
            handler->second->execute(arg);
        }
        else
        {
            L_WARNING << "Could not find handler for order '" << selected_order << "'.";
        }

        if (is_key_pressed(Command::Value::Multiple_Order) == false)
        {
            UI::getSingleton().reset();
        }
    }

    void Input::handle_skill(const OIS::MouseEvent& arg, const std::string& selected_skill)
    {
        PlayerPtr player = Game::getSingleton().get_active_player();

        if (player == nullptr)
        {
            return;
        }

        // TODO: Implement.
    }

    void Input::handle_selection(const OIS::MouseEvent& arg)
    {
        auto selection_box = UI::getSingleton().get_component<SelectionBoxWidget>();

        selection_box->set_end(Utils::Convert::to_screen_coordinates(arg));
        selection_box->set_visible(false);

        if (selection_box->is_valid())
        {
            if (PlayerPtr player = Game::getSingleton().get_active_player())
            {
                std::set<Id> objects = Utils::Raycast::to_objects(Game::getSingleton().get_map_scene(), selection_box->get_start(), selection_box->get_end());

                player->get_selected()->select(objects, is_key_pressed(Command::Value::Multiple_Select));
            }
        }
        else
        {
            if (PlayerPtr player = Game::getSingleton().get_active_player())
            {
                if (ObjectRaycastResults object_cast = Utils::Raycast::to_object(Game::getSingleton().get_map_scene(), arg))
                {
                    player->get_selected()->select(object_cast->first, is_key_pressed(Command::Value::Multiple_Select));
                }
                else
                {
                    player->get_selected()->select(Id::Empty, is_key_pressed(Command::Value::Multiple_Select));
                }
            }
        }
    }
}
