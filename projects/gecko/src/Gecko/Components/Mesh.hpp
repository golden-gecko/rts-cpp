#pragma once

#include "Gecko/Components/Component.hpp"

namespace Gecko
{
    class Mesh :
        public Component
    {
    private:
        using base_type = Component;

    public:
        // From Initializable.
        void init() override;

    public:
        static Mesh* create(Mesh* memory, const ConfigurationPtr& configuration, const ScenePtr& scene);

    public:
        ~Mesh() override;

    public:
        bool is_visible() const override
        {
            return m_entity->isVisible();
        }

    public:
        Ogre::Vector3 get_direction() const override;

        auto& get_entity()
        {
            return *m_entity;
        }

        const auto& get_entity() const
        {
            return *m_entity;
        }

        auto& get_scene_node()
        {
            return *m_scene_node;
        }

        const auto& get_scene_node() const
        {
            return *m_scene_node;
        }

        Ogre::Vector3 get_size() const override
        {
            return m_entity->getBoundingBox().getSize();
        }

    public:
        void set_owner(ObjectPtr owner) override;

        void set_position(const Ogre::Vector3& position) override;

        void set_visible(bool visible) override
        {
            base_type::set_visible(visible);

            m_scene_node->setVisible(visible);
        }

    private:
        Ogre::Entity*    m_entity     = nullptr;
        Ogre::SceneNode* m_scene_node = nullptr;

        void load_from_file();
        void load_from_vertices();
    };
}
