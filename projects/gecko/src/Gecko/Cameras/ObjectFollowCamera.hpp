#pragma once

#include "Gecko/Cameras/Camera.hpp"
#include "Gecko/Id.hpp"

namespace Gecko
{
    class ObjectFollowCamera :
        public Camera
    {
    private:
        using base_type = Camera;

    public:
        // From Serializable.
        ConfigurationPtr serialize() const override;
        void deserialize(const ConfigurationPtr& configuration) override;

    public:
        // From Camera.
        void update(float time) override;

    public:
        explicit ObjectFollowCamera(Ogre::Root* root, Ogre::SceneManager* scene_manager, const std::string& name, const ConfigurationPtr& configuration);

    public:
        float get_distance() const
        {
            return m_distance;
        }

        Id get_target_id() const
        {
            return m_target_id;
        }

    public:
        void set_distance(float distance)
        {
            m_distance = distance;
        }

        void set_target_id(Id target_id)
        {
            m_target_id = target_id;
        }

    private:
        Id m_target_id;
        float m_distance = 0.0f;
    };
}
