#pragma once

#include "Gecko/Interfaces/Initializable.hpp"
#include "Gecko/Interfaces/Updatable.hpp"
#include "Gecko/Orders/Order.hpp"
#include "Gecko/Timer.hpp"
#include "Gecko/UI/SelectionBox.hpp"

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

        ~UI() override;

        void render(Ogre::uint8 queueGroupId, const Ogre::String& cameraName, bool& skipThisInvocation);

    public:
        void change_visibility(const std::string& type, bool visible);

        /*
        void engine_application_save_options(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args);
        void engine_application_quit(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args);
        void engine_game_new(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args);
        void engine_game_load(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args);
        void engine_game_save(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args);
        void engine_game_quit(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args);
        void engine_map_set_data_layer(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args);
        void engine_minimap_click(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args);
        void engine_minimap_move(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args);
        void engine_minimap_zoom_in(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args);
        void engine_minimap_zoom_out(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args);
        void engine_preview_hide(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args);
        void engine_preview_set_position(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args);
        void engine_preview_show(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args);
        void engine_technology_research(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args);
        void engine_ui_change_visibility(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args);
        void engine_ui_look_at_object(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args);
        void engine_ui_select_object(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args);
        void engine_ui_set_configuration(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args);
        void engine_ui_set_order(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args);
        void engine_ui_set_skill(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args);
        */

        void inject_key_press(char key_code);
        void inject_key_release(char key_code);
        void inject_mouse_move(std::size_t x, std::size_t y);
        void inject_mouse_press(std::size_t x, std::size_t y, OIS::MouseButtonID id);
        void inject_mouse_release(std::size_t x, std::size_t y, OIS::MouseButtonID id);

        bool is_mouse_inside(std::size_t x, std::size_t y);

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
        const auto& get_configuration_name() const
        {
            return configuration_name;
        }

        auto& get_cursor()
        {
            return *cursor.get();
        }

        const auto& get_cursor() const
        {
            return *cursor.get();
        }

        Rml::ElementDocument* get_document() const
        {
            return document;
        }

        auto& get_minimap()
        {
            return *minimap.get();
        }

        const auto& get_minimap() const
        {
            return *minimap.get();
        }

        auto get_order_name() const
        {
            return order_name;
        }

        auto& get_preview()
        {
            return *preview.get();
        }

        const auto& get_preview() const
        {
            return *preview.get();
        }

        auto& get_selection_box()
        {
            return *selection_box.get();
        }

        const auto& get_selection_box() const
        {
            return *selection_box.get();
        }

        const auto& get_skill_name() const
        {
            return skill_name;
        }

    public:
        void set_configuration_name(const std::string& _configuration_name);
        void set_configurations(const std::set<std::string>& configurations);
        void set_configurations_header(const std::string& _configuration_name);
        void set_floating_descriptions(const std::vector<Id>& objects);
        void set_hovered_object_id(Id object_id);
        void set_info(const ConfigurationPtr& info);
        void set_layers(const std::map<std::string, std::shared_ptr<Layer>>& layers);
        void set_maps(const std::vector<std::string>& maps);
        void set_objects_admin();
        void set_orders_admin();
        void set_players();
        void set_order_name(order_type::Value _order_name);
        void set_orders(const std::set<std::string>& orders);
        void set_orders_header(order_type::Value order_type);
        void set_resources(std::shared_ptr<Resources> resources);
        void set_saves(const std::vector<std::string>& saves);
        void set_skill_name(const std::string& _order_name);
        void set_skills(const std::set<std::string>& skills);
        void set_skills_header(const std::string& skill_name);
        void set_statistics(const std::map<std::string, std::string>& info);
        void set_terrain_layers(const std::set<std::string>& layers);
        void set_visible(bool visible);
        void set_water_layers(const std::set<std::string>& layers);

    private:
        ConfigurationPtr m_configuration;

        std::shared_ptr<RenderInterface> render_interface;
        std::shared_ptr<SystemInterface> system_interface;

        Rml::Context* context = nullptr;
        Rml::ElementDocument* document = nullptr;

        // TODO: Rename element to body.
        Rml::Element* configurations_body = nullptr;
        Rml::Element* configurations_header = nullptr;
        Rml::Element* info_body = nullptr;
        Rml::Element* layers_body = nullptr;
        Rml::Element* log_body = nullptr;
        Rml::Element* maps_body = nullptr;
        Rml::Element* objects_admin_body = nullptr;
        Rml::Element* orders_admin_body = nullptr;
        Rml::Element* orders_body = nullptr;
        Rml::Element* orders_header = nullptr;
        Rml::Element* resources_body = nullptr;
        Rml::Element* skills_header = nullptr;
        Rml::Element* skills_body = nullptr;
        Rml::Element* statistics_body = nullptr;

        std::unique_ptr<Cursor> cursor;
        std::unique_ptr<Minimap> minimap;
        std::unique_ptr<Preview> preview;
        std::unique_ptr<SelectionBox> selection_box;

        Id hovered_object_id;

        bool floating_description = false;
        bool m_visible = true;

        Timer refresh_time = Timer(Settings::UI::RefreshInterval);

        std::string configuration_name = "none";
        order_type::Value order_name = order_type::Value::None;
        std::string skill_name = "none";

        std::shared_ptr<EventListenerInstancer> event_listener_instancer;

        void init_components();
        void init_documents();
        void init_events();
        void init_fonts();
        void init_visibility_types();

        void log_write(const std::string& text, const std::string& type, Id id = Id::Empty);

        Rml::Element* get_header_placeholder(const std::string& selector) const;
        Rml::Element* get_body_placeholder(const std::string& selector) const;
    };
}
