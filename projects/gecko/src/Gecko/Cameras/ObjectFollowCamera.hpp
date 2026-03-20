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

    // From Serializable.
    public:
        ConfigurationPtr serialize() const override;
        void deserialize(const ConfigurationPtr& configuration) override;

    // From Camera.
    public:
        void update(float time) override;

    // ObjectFollowCamera.
    public:
        explicit ObjectFollowCamera(const std::string& name, const ConfigurationPtr& configuration, Scene* scene);

    public:
        const Id& get_target_id() const
        {
            return m_target_id;
        }

        float get_distance() const
        {
            return m_distance;
        }

        float get_zoom_step() const
        {
            return m_zoom_step;
        }

    public:
        void set_distance(float distance)
        {
            m_distance = distance;
        }

        void set_target_id(const Id& target_id)
        {
            m_target_id = target_id;
        }

    private:
        Id m_target_id;

        float m_distance  = 0.0f;
        float m_zoom_step = 0.0f;
    };
}
