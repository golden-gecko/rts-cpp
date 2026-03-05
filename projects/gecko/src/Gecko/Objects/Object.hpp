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
        static Object* create(Object* memory, const ConfigurationPtr& configuration, const ScenePtr& scene);

    public:
        explicit Object(const ScenePtr& scene);
        explicit Object(const Object& other);

        ~Object() override;

    public:
        Object& operator=(const Object&) = delete;

    public:
        bool is_selected() const
        {
            return m_selected;
        }

        bool is_visible() const
        {
            return m_visible;
        }

    public:
        const auto& get_components() const
        {
            return m_components;
        }

        const auto& get_configurations() const
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

        const auto& get_orders() const
        {
            return m_orders;
        }

        const ProcessesPtr& get_processes() const
        {
            return m_processes;
        }

        const ResourcesPtr& get_resources() const
        {
            return m_resources;
        }

        MapPtr get_owner() const
        {
            return m_owner;
        }

        const auto& get_player_id() const
        {
            return m_player_id;
        }

        const Ogre::Vector3& get_position() const;

        virtual std::map<std::string, float> get_progress_bars() const;

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

        const auto& get_skills() const
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

        void set_owner(MapPtr _owner)
        {
            m_owner = _owner;
        }

        virtual void set_player_id(const Id& player_id);

        virtual void set_position(const Ogre::Vector3& position, bool validate = true);

        virtual void set_scale(const Ogre::Vector3& scale)
        {
            m_scene_node->setScale(scale);
        }

        virtual void set_selected(bool selected);

        virtual void set_visible(bool visible);

    protected:
        virtual void update_components(float time);
        virtual void update_jobs(float time);
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
        MapPtr   m_owner = nullptr;
        ScenePtr m_scene;

        std::string m_name;
        Id m_player_id;
        Timer m_alive_timer;

        bool m_selected = false;
        bool m_visible = false;

        ComponentsPtr m_components;
        ConfigurationsPtr m_configurations;
        ProcessesPtr m_processes;
        ResourcesPtr m_resources;
        OrdersPtr m_orders;
        LayersPtr m_layers;
        SkillsPtr m_skills;
        std::shared_ptr<Mesh> m_selection;

        Ogre::SceneNode* m_scene_node = nullptr;

        // TODO: Move to serialize and deserialize methods.
        Timer m_job_timer = Timer(1.0f);

        void create_selection_mesh();
    };
}
