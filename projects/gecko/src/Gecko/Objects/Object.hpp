#pragma once

#include "Gecko/Area.hpp"
#include "Gecko/Entrance.hpp"
#include "Gecko/Item.hpp"
#include "Gecko/Orders/Order.hpp"
#include "Gecko/Timer.hpp"
#include "Gecko/Utils/Utils.hpp"

namespace Gecko
{
    class Object :
        public Item
    {
    private:
        using base_type = Item;

    public:
        static Object* create(Object* memory, const std::shared_ptr<Configuration>& configuration);

    public:
        explicit Object();
        explicit Object(const Object& other);

        ~Object() override;

        void init() override;
        void deinit() override;

        std::shared_ptr<Configuration> serialize() const override;
        void deserialize(const std::shared_ptr<Configuration>& configuration) override;

        void update(float time) override;

    public:
        auto is_selectable() const
        {
            return selectable;
        }

        auto is_selected() const
        {
            return selected;
        }

        auto is_visible() const
        {
            return visible;
        }

    public:
        auto get_components() const
        {
            return components;
        }

        auto get_configurations() const
        {
            return configurations;
        }

        auto get_direction() const
        {
            return Utils::get_node_direction(*scene_node);
        }

        virtual Entrance get_entrance() const;

        float get_heading() const;

        std::shared_ptr<Configuration> get_info() const;

        const auto& get_name() const
        {
            return name;
        }

        auto get_orders() const
        {
            return orders;
        }

        Map* get_owner() const
        {
            return owner;
        }

        const auto& get_player_id() const
        {
            return player_id;
        }

        const Ogre::Vector3& get_position() const
        {
            return Utils::get_node_position(*scene_node);
        }

        auto get_processes() const
        {
            return processes;
        }

        virtual std::map<std::string, float> get_progress_bars() const;

        auto get_resources() const
        {
            return resources;
        }

        const Ogre::Vector3& get_scale() const
        {
            return scene_node->getScale();
        }

        auto& get_scene_node()
        {
            return *scene_node;
        }

        const auto& get_scene_node() const
        {
            return *scene_node;
        }

        Area get_size() const;

        auto get_skills() const
        {
            return skills;
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
            scene_node->setDirection(direction, Ogre::Node::TransformSpace::TS_WORLD);
        }

        void set_owner(Map* _owner)
        {
            owner = _owner;
        }

        virtual void set_player_id(Id player_id);

        virtual void set_position(const Ogre::Vector3& position, bool validate = true);

        virtual void set_scale(const Ogre::Vector3& scale)
        {
            scene_node->setScale(scale);
        }

        virtual void set_selected(bool _selected);

        virtual void set_visible(bool _visible);

    protected:
        virtual void update_components(float time);
        virtual void update_orders(float time);
        virtual void update_processes(float time);
        virtual void update_resources(float time);
        virtual void update_skills(float time);

        // TODO: Move to order resolver.
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
        // TODO: End.

    private:
        Map* owner = nullptr;

        std::string name;
        Id player_id;
        Timer alive_timer;

        bool selectable = false;
        bool selected = false;
        bool visible = false;

        std::shared_ptr<Components> components;
        std::shared_ptr<Configurations> configurations;
        std::shared_ptr<Orders> orders;
        std::shared_ptr<Layers> layers;
        std::shared_ptr<Processes> processes;
        std::shared_ptr<Resources> resources;
        std::shared_ptr<Skills> skills;
        std::shared_ptr<Mesh> selection;

        Ogre::SceneNode* scene_node = nullptr;

        // TODO: Move to serialize and deserialize methods.
        Timer job_timer = Timer(1.0f);

        void create_selection_mesh();
    };
}
