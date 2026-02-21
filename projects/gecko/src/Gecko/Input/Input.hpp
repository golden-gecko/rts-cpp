#pragma once

#include "Gecko/Input/Command.hpp"
#include "Gecko/Interfaces/Initializable.hpp"
#include "Gecko/Interfaces/Updatable.hpp"
#include "Gecko/Orders/Order.hpp"
#include "Gecko/Settings.hpp"

namespace Gecko
{
    class Input :
        public Ogre::Singleton<Input>,
        public OIS::JoyStickListener,
        public OIS::KeyListener,
        public OIS::MouseListener,
        public Initializable,
        public Updatable
    {
    public:
        // From OIS::JoyStickListener.
        bool buttonPressed(const OIS::JoyStickEvent& arg, int button) override;
        bool buttonReleased(const OIS::JoyStickEvent& arg, int button) override;
        bool axisMoved(const OIS::JoyStickEvent& arg, int axis) override;
        bool sliderMoved(const OIS::JoyStickEvent& arg, int index) override;
        bool povMoved(const OIS::JoyStickEvent& arg, int index) override;
        bool vector3Moved(const OIS::JoyStickEvent& arg, int index) override;

    public:
        // From OIS::KeyListener.
        bool keyPressed(const OIS::KeyEvent& arg) override;
        bool keyReleased(const OIS::KeyEvent& arg) override;

    public:
        // From OIS::MouseListener.
        bool mouseMoved(const OIS::MouseEvent& arg) override;
        bool mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id) override;
        bool mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id) override;

    public:
        // From Initializable.
        void init() override;
        void deinit() override;

    public:
        // From Updatable.
        void update(float time) override;

    public:
        explicit Input(const ConfigurationPtr& configuration, std::uint64_t render_window_handle);

        ~Input() override = default;

    public:
        bool is_key_pressed(Command::Value command) const;
        bool is_key_pressed(OIS::KeyCode key_code) const;

        bool is_mouse_button_pressed(OIS::MouseButtonID mouse_button) const;

    public:
        void set_mouse_sensitivity(const Ogre::Vector2& mouse_sensitivity);
        void set_window_size(int width, int height);

    private:
        ConfigurationPtr m_configuration;

        std::uint64_t m_render_window_handle = 0;

        OIS::InputManager* m_input_manager = nullptr;
        OIS::JoyStick*     m_joystick      = nullptr;
        OIS::Keyboard*     m_keyboard      = nullptr;
        OIS::Mouse*        m_mouse         = nullptr;

        std::map<Command::Value, std::set<OIS::KeyCode>> m_commands;

        Ogre::Vector2 m_mouse_sensitivity = Settings::Input::MouseSensivity;

        void handle_left_mouse_button(const OIS::MouseEvent& arg);
        void handle_right_mouse_button(const OIS::MouseEvent& arg);

        void handle_configuration(const OIS::MouseEvent& arg, const std::string& selected_configuration);
        void handle_order(const OIS::MouseEvent& arg, const std::string& selected_order);
        void handle_skill(const OIS::MouseEvent& arg, const std::string& selected_skill);
        void handle_selection(const OIS::MouseEvent& arg);
    };
}
