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
        explicit UI(const ConfigurationPtr& configuration);

        ~UI() override = default;

        void render(Ogre::uint8 queueGroupId, const Ogre::String& cameraName, bool& skipThisInvocation);

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

        void log_error(const std::string& text, Id id = Id::Empty);
        void log_info(const std::string& text, Id id = Id::Empty);

        void reset();
        void reset_configuration();
        void reset_order();
        void reset_skill();

        void restore_visibility(const std::map<std::string, bool>& state);

        void show_layers(bool visible);

        void show_menu();

        void toggle_floating_description();

    public:
        std::shared_ptr<ConfigurationsWidget>& get_configurations()
        {
            return m_configurations;
        }

        std::shared_ptr<CursorWidget>& get_cursor()
        {
            return m_cursor;
        }

        std::shared_ptr<InfoWidget>& get_info()
        {
            return m_info;
        }

        std::shared_ptr<MinimapWidget>& get_minimap()
        {
            return m_minimap;
        }

        std::shared_ptr<OrdersWidget>& get_orders()
        {
            return m_orders;
        }

        std::shared_ptr<PreviewWidget>& get_preview()
        {
            return m_preview;
        }

        std::shared_ptr<SelectionBoxWidget>& get_selection_box()
        {
            return m_selection_box;
        }

        std::shared_ptr<SkillsWidget>& get_skills()
        {
            return m_skills;
        }

        std::string get_configuration_name() const
        {
            return ""; // m_configuration_name;
        }

        Rml::ElementDocument* get_document() const
        {
            return m_document;
        }

        std::string get_order_name() const
        {
            return ""; // m_order_name;
        }

        auto get_order_type() const
        {
            return order_type::Value::None;
        }

        std::string get_skill_name() const
        {
            return ""; // m_skill_name;
        }

    public:
        /*
        void set_diplomacy();
        void set_floating_descriptions();
        */
        void set_hovered_object_id(const Id& hovered_object_id);
        void set_info(const ConfigurationPtr& info);
        /*
        void set_layers(const std::map<std::string, std::shared_ptr<Layer>>& layers);
        void set_maps(const std::vector<std::string>& maps);
        void set_objects_admin();
        void set_orders_admin();
        void set_saves(const std::vector<std::string>& saves);
        void set_terrain_layers(const std::set<std::string>& layers);
        void set_visible(bool visible);
        void set_water_layers(const std::set<std::string>& layers);
        */

    private:
        ConfigurationPtr m_configuration;

        std::shared_ptr<RenderInterface> m_render_interface;
        std::shared_ptr<SystemInterface> m_system_interface;

        Rml::Context* m_context = nullptr;
        Rml::ElementDocument* m_document = nullptr;

        std::shared_ptr<ConfigurationsWidget> m_configurations;
        std::shared_ptr<ConsoleWidget>        m_console;
        std::shared_ptr<CursorWidget>         m_cursor;
        std::shared_ptr<DiplomacyWidget>      m_diplomacy;
        std::shared_ptr<InfoWidget>           m_info;
        std::shared_ptr<LogWidget>            m_log;
        std::shared_ptr<MinimapWidget>        m_minimap;
        std::shared_ptr<OrdersWidget>         m_orders;
        std::shared_ptr<PlayersWidget>        m_players;
        std::shared_ptr<PreviewWidget>        m_preview;
        std::shared_ptr<ResourcesWidget>      m_resources;
        std::shared_ptr<SkillsWidget>         m_skills;
        std::shared_ptr<StatisticsWidget>     m_statistics;

        std::shared_ptr<SelectionBoxWidget>           m_selection_box;
        std::vector<std::shared_ptr<Indicator>> m_indicators;

        void refresh_indicators(const Id& id);

        Id m_hovered_object_id;

        bool m_floating_description = false;
        bool m_visible = true;

        Timer m_refresh_time = Timer(Settings::UI::RefreshInterval);

        std::shared_ptr<EventListenerInstancer> m_event_listener_instancer;

        void init_components();
        void init_data_bindings();
        void init_documents();
        void init_events();
        void init_fonts();
        void init_visibility_types();

        void log_write(const std::string& message, const std::string& type, const Id& id = Id::Empty);
    };
}
