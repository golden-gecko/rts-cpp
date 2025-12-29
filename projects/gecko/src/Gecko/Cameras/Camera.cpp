#include "Gecko/Cameras/Camera.hpp"

#include "Gecko/Configuration.hpp"
#include "Gecko/Input.hpp"
#include "Gecko/Managers/MapManager.hpp"
#include "Gecko/Maps/Map.hpp"

namespace Gecko
{
    Camera::Camera(Ogre::Root* root, Ogre::SceneManager* scene_manager, const std::string& name, const std::shared_ptr<Configuration>& configuration) :
        m_scene_manager(scene_manager)
    {
        auto near_clip_distance = configuration->get_float(
            "options.near_clip_distance", Settings::Camera::NearClipDistance
        );

        m_camera = m_scene_manager->createCamera(name);
        m_camera->setAutoAspectRatio(true);
        m_camera->setNearClipDistance(near_clip_distance);

        m_camera_scene_node = m_scene_manager->createSceneNode(name);
        m_camera_scene_node->attachObject(m_camera);

        // TODO: Use deserialize instead of constructor.
        deserialize(configuration);

        auto polygon_mode = configuration->get_string("polygon_mode", Settings::Camera::PolygonMode);

        if (polygon_mode == "points")
        {
            m_camera->setPolygonMode(Ogre::PolygonMode::PM_POINTS);
        }
        else if (polygon_mode == "solid")
        {
            m_camera->setPolygonMode(Ogre::PolygonMode::PM_SOLID);
        }
        else if (polygon_mode == "wireframe")
        {
            m_camera->setPolygonMode(Ogre::PolygonMode::PM_WIREFRAME);
        }

        auto projection_type = configuration->get_string("projection_type", Settings::Camera::ProjectionType);

        if (projection_type == "orthographic")
        {
            m_camera->setProjectionType(Ogre::ProjectionType::PT_ORTHOGRAPHIC);
        }
        else if (projection_type == "perspective")
        {
            m_camera->setProjectionType(Ogre::ProjectionType::PT_PERSPECTIVE);
        }

        if (root->getRenderSystem()->getCapabilities()->hasCapability(Ogre::Capabilities::RSC_INFINITE_FAR_PLANE))
        {
            m_camera->setFarClipDistance(0.0f);
        }
        else
        {
            auto far_clip_distance = configuration->get_float(
                "options.far_clip_distance", Settings::Camera::FarClipDistance
            );

            m_camera->setFarClipDistance(far_clip_distance);
        }
    }

    Camera::~Camera()
    {
        m_scene_manager->destroyCamera(m_camera);
    }

    std::shared_ptr<Configuration> Camera::serialize() const
    {
        auto configuration = std::make_shared<Configuration>();

        configuration->set("direction", get_direction());
        configuration->set("position", get_position());
        configuration->set("speed", m_speed);

        return configuration;
    }

    void Camera::deserialize(const std::shared_ptr<Configuration>& configuration)
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
