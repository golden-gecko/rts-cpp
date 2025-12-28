#pragma once

#include "Gecko/Components/Component.hpp"
#include "Gecko/Utils/Utils.hpp"

namespace Gecko
{
    class Mesh : public Component
    {
    private:
        using base_type = Component;

    public:
        static Mesh* create(Mesh* memory, const std::shared_ptr<Configuration>& configuration);

    public:
        explicit Mesh() = default;
        explicit Mesh(const Mesh& other);

        ~Mesh() override;

        void init() override;

    public:
        bool is_visible() const override
        {
            return entity->isVisible();
        }

    public:
        Ogre::Vector3 get_direction() const override
        {
            return Utils::get_node_direction(*scene_node);
        }

        auto& get_entity()
        {
            return *entity;
        }

        const auto& get_entity() const
        {
            return *entity;
        }

        auto& get_scene_node()
        {
            return *scene_node;
        }

        const auto& get_scene_node() const
        {
            return *scene_node;
        }

        Ogre::Vector3 get_size() const override
        {
            return entity->getBoundingBox().getSize();
        }

    public:
        void set_owner(Object* _owner) override;

        void set_position(const Ogre::Vector3& position) override;

        void set_visible(bool visible) override
        {
            base_type::set_visible(visible);

            scene_node->setVisible(visible);
        }

    private:
        Ogre::Entity* entity = nullptr;
        Ogre::SceneNode* scene_node = nullptr;

        void load_from_file();
        void load_from_vertices();
    };
}
