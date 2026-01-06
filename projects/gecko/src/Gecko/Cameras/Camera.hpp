#pragma once

#include "Gecko/Interfaces/Serializable.hpp"
#include "Gecko/Interfaces/Updatable.hpp"

namespace Gecko
{
    class Camera :
        public Serializable,
        public Updatable
    {
    public:
        // From Serializable.
        std::shared_ptr<Configuration> serialize() const override;
        void deserialize(const std::shared_ptr<Configuration>& configuration) override;

    public:
        // From Updatable.
        void update(float time) override;

    public:
        explicit Camera(Ogre::Root* root, Ogre::SceneManager* scene_manager, const std::string& name, const std::shared_ptr<Configuration>& configuration);

        ~Camera() override;

    public:
        virtual void move(const Ogre::Vector3& direction);
        virtual void rotate(const Ogre::Degree& yaw_angle, const Ogre::Degree& pitch_angle);

        virtual void pitch(const Ogre::Degree& angle);
        virtual void yaw(const Ogre::Degree& angle);

    public:
        auto get_camera() const
        {
            return m_camera;
        }

        auto get_camera_node() const
        {
            return m_camera_scene_node;
        }

        auto get_direction() const
        {
            return m_camera_scene_node->getOrientation().zAxis();
        }

        const auto& get_position() const
        {
            return m_camera_scene_node->getPosition();
        }

        const auto& get_speed() const
        {
            return m_speed;
        }

    public:
        void set_position(const Ogre::Vector3& position)
        {
            m_camera_scene_node->setPosition(position);
        }

    protected:
        Ogre::SceneManager* m_scene_manager = nullptr;
        Ogre::Camera* m_camera = nullptr;
        Ogre::SceneNode* m_camera_scene_node = nullptr;

        Ogre::Vector3 m_speed = Ogre::Vector3::ZERO;
    };
}
