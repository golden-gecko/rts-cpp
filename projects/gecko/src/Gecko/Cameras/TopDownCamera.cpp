#include "Gecko/Cameras/TopDownCamera.hpp"

#include "Gecko/Input.hpp"
#include "Gecko/Layers/SquareLayer.hpp"
#include "Gecko/Managers/MapManager.hpp"
#include "Gecko/Maps/Map.hpp"

namespace Gecko
{
    TopDownCamera::TopDownCamera(Ogre::Root* root, Ogre::SceneManager* scene_manager, const std::string& name, const ConfigurationPtr& configuration) :
        base_type(root, scene_manager, name, configuration)
    {
    }

    void TopDownCamera::update(float time)
    {
        Ogre::Vector3 direction = Ogre::Vector3::ZERO;

        if (Input::getSingleton().is_key_pressed(Command::Value::Camera_Move_Left))
        {
            direction += Ogre::Vector3::NEGATIVE_UNIT_X;
        }
        else if (Input::getSingleton().is_key_pressed(Command::Value::Camera_Move_Right))
        {
            direction += Ogre::Vector3::UNIT_X;
        }

        if (Input::getSingleton().is_key_pressed(Command::Value::Camera_Move_Down))
        {
            direction += Ogre::Vector3::NEGATIVE_UNIT_Y;
        }
        else if (Input::getSingleton().is_key_pressed(Command::Value::Camera_Move_Up))
        {
            direction += Ogre::Vector3::UNIT_Y;
        }

        if (Input::getSingleton().is_key_pressed(Command::Value::Camera_Move_Forward))
        {
            direction += Ogre::Vector3::NEGATIVE_UNIT_Z;
        }
        else if (Input::getSingleton().is_key_pressed(Command::Value::Camera_Move_Backward))
        {
            direction += Ogre::Vector3::UNIT_Z;
        }

        move(direction * m_speed * time);

        // Limit camera to layer.
        // TODO: First map.
        // TODO: Hardcoded layer name.
        auto layer = MapManager::getSingleton().begin()->second->get_layer("Terrain");

        if (layer)
        {
            auto camera_position = get_position();
            auto layer_position = layer->get_position(camera_position);

            auto min_x = layer->get_scale().x;
            auto max_x = layer->get_size() * layer->get_scale().x - layer->get_scale().x;

            auto min_y = layer_position.y + Settings::Camera::MinHeight;
            auto max_y = layer_position.y + Settings::Camera::MaxHeight;

            auto min_z = layer->get_scale().z;
            auto max_z = layer->get_size() * layer->get_scale().z - layer->get_scale().z;

            camera_position.x = std::clamp(camera_position.x, min_x, max_x);
            camera_position.y = std::clamp(camera_position.y, min_y, max_y);
            camera_position.z = std::clamp(camera_position.z, min_z, max_z);

            set_position(camera_position);
        }
    }
}
