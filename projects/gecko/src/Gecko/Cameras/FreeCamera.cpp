#include "Gecko/Cameras/FreeCamera.hpp"

#include "Gecko/Input/Input.hpp"

namespace Gecko
{
    void FreeCamera::update(float time)
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
    }

    FreeCamera::FreeCamera(Scene* scene, const std::string& name, const ConfigurationPtr& configuration) :
        base_type(scene, name, configuration)
    {
    }
}
