#include "Cameras/FreeCamera.hpp"

#include "Input.hpp"

namespace Gecko
{
    FreeCamera::FreeCamera(Ogre::Root* root, Ogre::SceneManager* scene_manager, const std::string& name, const std::shared_ptr<Configuration>& configuration) :
        base_type(root, scene_manager, name, configuration)
    {
    }

    void FreeCamera::update(float time)
    {
        auto direction = Ogre::Vector3::ZERO;

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

        m_camera_node->translate(direction * m_speed * time);
    }
}
