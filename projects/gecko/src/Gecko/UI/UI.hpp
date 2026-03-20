#pragma once

#include "Gecko/Interfaces/Initializable.hpp"
#include "Gecko/Interfaces/Updatable.hpp"
#include "Gecko/Orders/Order.hpp"
#include "Gecko/Timer.hpp"

namespace Gecko
{
    class UI :
        public Ogre::Singleton<UI>,
        public Initializable,
        public Updatable
    {
    public:
        // From Initializable.
        void init() override;
        void deinit() override;

    public:
        // From Updatable.
        void update(float time) override;

    public:
        explicit UI(const ConfigurationPtr& configuration, const ScenePtr& scene);

        ~UI() override = default;

    public:
        void change_visibility(const std::string& type, bool visible);

        bool inject_key_press(Rml::Input::KeyIdentifier key);
        bool inject_key_release(Rml::Input::KeyIdentifier key);
        bool inject_text(unsigned int text);
        bool inject_mouse_move(int x, int y, int z);
        bool inject_mouse_press(int button);
        bool inject_mouse_release(int button);

        bool is_mouse_inside(int x, int y);

        bool is_visible() const
        {
            return m_visible;
        }

        void log_error(const std::string& text, const Id& id = Id::Empty);
        void log_info(const std::string& text, const Id& id = Id::Empty);

        void reset();
        void reset_configurations();
        void reset_orders();
        void reset_skills();

    public:
        template<typename T>
        std::shared_ptr<T> get_component()
        {
            for (const auto& i : m_widgets)
            {
                std::shared_ptr<T> component = std::dynamic_pointer_cast<T>(i);

                if (component != nullptr)
                {
                    return component;
                }
            }

            return nullptr;
        }

        Rml::Context* get_context() const
        {
            return m_context;
        }

        Rml::ElementDocument* get_document() const
        {
            return m_document;
        }

        const std::shared_ptr<RenderInterface>& get_render_interface() const
        {
            return m_render_interface;
        }

        void set_hovered_object_id(const Id& hovered_object_id);

    private:
        ConfigurationPtr m_configuration;
        ScenePtr         m_scene;

        std::shared_ptr<RenderInterface> m_render_interface;
        std::shared_ptr<SystemInterface> m_system_interface;

        Rml::Context*         m_context  = nullptr;
        Rml::ElementDocument* m_document = nullptr;

        std::vector<std::shared_ptr<Widget>>    m_widgets;
        std::vector<std::shared_ptr<Indicator>> m_indicators;

        void refresh_indicators(const Id& id);

        Id m_hovered_object_id;

        bool m_floating_description = false;
        bool m_visible = true;

        Timer m_refresh_time = Timer(Settings::UI::RefreshInterval);

        std::shared_ptr<EventListenerInstancer> m_event_listener_instancer;

        void init_data_bindings();
        void init_documents();
        void init_events();
        void init_fonts();
        void init_widgets();

        void deinit_documents();
        void deinit_events();
        void deinit_widgets();

        void log_write(const std::string& message, const std::string& type, const Id& id = Id::Empty);
    };
}
