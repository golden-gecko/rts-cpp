#pragma once

#include "Gecko/Area.hpp"
#include "Gecko/Entrance.hpp"
#include "Gecko/Item.hpp"
#include "Gecko/Orders/Order.hpp"
#include "Gecko/Timer.hpp"

namespace Gecko
{
    class Object :
        public Item
    {
    private:
        using base_type = Item;

    public:
        // From Initializable.
        void init() override;
        void deinit() override;

    public:
        // From Serializable.
        ConfigurationPtr serialize() const override;
        void deserialize(const ConfigurationPtr& configuration) override;

    public:
        // From Updatable.
        void update(float time) override;

    public:
        static Object* create(Object* memory, const ConfigurationPtr& configuration);

    public:
        explicit Object();
        explicit Object(const Object& other);

        ~Object() override;

    public:
        auto is_selectable() const
        {
            return m_selectable;
        }

        auto is_selected() const
        {
            return m_selected;
        }

        auto is_visible() const
        {
            return m_visible;
        }

    public:
        auto get_components() const
        {
            return m_components;
        }

        auto get_configurations() const
        {
            return m_configurations;
        }

        Ogre::Vector3 get_direction() const;

        virtual Entrance get_entrance() const;

        float get_heading() const;

        ConfigurationPtr get_info() const;

        const auto& get_name() const
        {
            return m_name;
        }

        auto get_orders() const
        {
            return m_orders;
        }

        Map* get_owner() const
        {
            return m_owner;
        }

        const auto& get_player_id() const
        {
            return m_player_id;
        }

        const Ogre::Vector3& get_position() const;

        auto get_processes() const
        {
            return m_processes;
        }

        virtual std::map<std::string, float> get_progress_bars() const;

        auto get_resources() const
        {
            return m_resources;
        }

        const Ogre::Vector3& get_scale() const
        {
            return m_scene_node->getScale();
        }

        auto& get_scene_node()
        {
            return *m_scene_node;
        }

        const auto& get_scene_node() const
        {
            return *m_scene_node;
        }

        Area get_area() const;

        Ogre::Vector3 get_size() const;

        auto get_skills() const
        {
            return m_skills;
        }

        // TODO: Implement from components.
        /*
        auto get_visibility_range() const
        {
            return visibility_range;
        }
        */

    public:
        virtual void set_direction(const Ogre::Vector3& direction)
        {
            m_scene_node->setDirection(direction, Ogre::Node::TransformSpace::TS_WORLD);
        }

        void set_owner(Map* _owner)
        {
            m_owner = _owner;
        }

        virtual void set_player_id(Id player_id);

        virtual void set_position(const Ogre::Vector3& position, bool validate = true);

        virtual void set_scale(const Ogre::Vector3& scale)
        {
            m_scene_node->setScale(scale);
        }

        virtual void set_selected(bool selected);

        virtual void set_visible(bool visible);

    protected:
        virtual void update_components(float time);
        virtual void update_orders(float time);
        virtual void update_processes(float time);
        virtual void update_resources(float time);
        virtual void update_skills(float time);

        virtual OrderStatus on_attack(Order* order, float time);
        virtual OrderStatus on_create(Order* order, float time);
        virtual OrderStatus on_destroy(Order* order, float time);
        virtual OrderStatus on_disable(Order* order, float time);
        virtual OrderStatus on_enable(Order* order, float time);
        virtual OrderStatus on_follow(Order* order, float time);
        virtual OrderStatus on_guard(Order* order, float time);
        virtual OrderStatus on_load(Order* order, float time);
        virtual OrderStatus on_move(Order* order, float time);
        virtual OrderStatus on_patrol(Order* order, float time);
        virtual OrderStatus on_rally(Order* order, float time);
        virtual OrderStatus on_stop(Order* order, float time);
        virtual OrderStatus on_unload(Order* order, float time);
        virtual OrderStatus on_wait(Order* order, float time);

    private:
        Map* m_owner = nullptr;

        std::string m_name;
        Id m_player_id;
        Timer m_alive_timer;

        bool m_selectable = false;
        bool m_selected = false;
        bool m_visible = false;

        std::shared_ptr<Components> m_components;
        std::shared_ptr<Configurations> m_configurations;
        std::shared_ptr<Orders> m_orders;
        std::shared_ptr<Layers> m_layers;
        std::shared_ptr<Processes> m_processes;
        std::shared_ptr<Resources> m_resources;
        std::shared_ptr<Skills> m_skills;
        std::shared_ptr<Mesh> m_selection;

        Ogre::SceneNode* m_scene_node = nullptr;

        // TODO: Move to serialize and deserialize methods.
        Timer m_job_timer = Timer(1.0f);

        void create_selection_mesh();
    };
}
