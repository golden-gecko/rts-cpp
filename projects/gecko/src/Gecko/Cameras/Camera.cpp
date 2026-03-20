#include "Gecko/Cameras/Camera.hpp"

#include "Gecko/Configuration.hpp"
#include "Gecko/Utils/Convert.hpp"
#include "Gecko/Input/Input.hpp"
#include "Gecko/Managers/MapManager.hpp"
#include "Gecko/Maps/Map.hpp"
#include "Gecko/Scenes/Scene.hpp"

namespace Gecko
{
    Camera::Camera(const std::string& name, const ConfigurationPtr& configuration, Scene* scene) :
        m_name(name),
        m_configuration(configuration),
        m_scene(scene)
    {
        m_camera = m_scene->create_camera(name);
        m_camera->setAutoAspectRatio(true);
        m_camera->setFarClipDistance(configuration->get_float("far_clip_distance", Settings::Camera::FarClipDistance));
        m_camera->setNearClipDistance(configuration->get_float("near_clip_distance", Settings::Camera::NearClipDistance));
        m_camera->setPolygonMode(Utils::Convert::to_polygon_mode(configuration->get_string("polygon_mode", Settings::Camera::PolygonMode)));
        m_camera->setProjectionType(Utils::Convert::to_projection_type(configuration->get_string("projection_type", Settings::Camera::ProjectionType)));

        m_camera_scene_node = m_scene->create_scene_node();
        m_camera_scene_node->attachObject(m_camera);
        m_camera_scene_node->setFixedYawAxis(true);

        // TODO: Remove this from constructor. Does not work.
        deserialize(configuration);
    }

    Camera::~Camera()
    {
        m_scene->destroy_scene_node(m_camera_scene_node);
        m_scene->destroy_camera(m_camera);
    }

    ConfigurationPtr Camera::serialize() const
    {
        ConfigurationPtr configuration = std::make_shared<Configuration>();

        configuration->set("direction", get_direction());
        configuration->set("position", get_position());
        configuration->set("speed", m_speed);

        return configuration;
    }

    void Camera::deserialize(const ConfigurationPtr& configuration)
    {
        m_camera_scene_node->setPosition(configuration->get_vector3("position", Ogre::Vector3::ZERO));
        m_camera_scene_node->lookAt(configuration->get_vector3("look_at", Ogre::Vector3::ZERO), Ogre::Node::TransformSpace::TS_PARENT);

        m_speed = configuration->get_vector3("speed", Ogre::Vector3::ZERO);
    }

    void Camera::update(float time)
    {
    }

    void Camera::move(const Ogre::Vector3& direction)
    {
        m_camera_scene_node->translate(direction, Ogre::Node::TransformSpace::TS_LOCAL);
    }

    void Camera::rotate(const Ogre::Degree& yaw_angle, const Ogre::Degree& pitch_angle)
    {
        yaw(yaw_angle);
        pitch(pitch_angle);
    }

    void Camera::yaw(const Ogre::Degree& angle)
    {
        m_camera_scene_node->yaw(angle, Ogre::Node::TS_WORLD);
    }

    void Camera::pitch(const Ogre::Degree& angle)
    {
        m_camera_scene_node->pitch(angle, Ogre::Node::TS_LOCAL);
    }
}
