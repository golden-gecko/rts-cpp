#pragma once

#include "Interfaces/Serializable.hpp"
#include "Interfaces/Updatable.hpp"

namespace Gecko
{
    class Camera :
        public Serializable,
        public Updatable
    {
    public:
        explicit Camera(Ogre::Root* root, Ogre::SceneManager* scene_manager, const std::string& name, const std::shared_ptr<Configuration>& configuration);

        ~Camera() override;

        std::shared_ptr<Configuration> serialize() const override;
        void deserialize(const std::shared_ptr<Configuration>& configuration) override;

        void update(float time) override;

    public:
        virtual void pitch(const Ogre::Radian& angle);
        virtual void yaw(const Ogre::Radian& angle);

    public:
        auto get_camera() const
        {
            return m_camera;
        }

        auto get_direction() const
        {
            return m_camera->getDirection();
        }

        const auto& get_position() const
        {
            return m_camera->getPosition();
        }

        const auto& get_speed() const
        {
            return m_speed;
        }

    public:
        void set_position(const Ogre::Vector3& position)
        {
            m_camera->setPosition(position);
        }

    protected:
        Ogre::SceneManager* m_scene_manager = nullptr;
        Ogre::Camera* m_camera = nullptr;

        Ogre::Vector3 m_speed = Ogre::Vector3::ZERO;
    };
}
