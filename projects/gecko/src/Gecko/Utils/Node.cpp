#include "Gecko/Utils/Node.hpp"

namespace Gecko::Utils::Node
{
    Ogre::Vector3 get_direction(Ogre::SceneNode* scene_node)
    {
        return scene_node->_getDerivedOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z;
    }

    const Ogre::Vector3& get_position(Ogre::SceneNode* scene_node)
    {
        return scene_node->_getDerivedPosition();
    }

    void rotate_towards_position(Ogre::SceneNode* scene_node, const Ogre::Vector3& position, float time)
    {
        Ogre::Vector3 current_position = get_position(scene_node);
        current_position.y = 0.0f;

        Ogre::Vector3 current_direction = get_direction(scene_node);
        current_direction.y = 0.0f;
        current_direction.normalise();

        Ogre::Vector3 target_direction = position - current_position;
        Ogre::Quaternion target_rotation = current_direction.getRotationTo(target_direction);

        float y = target_rotation.getYaw().valueDegrees();
        float p = target_rotation.getPitch().valueDegrees();

        if (std::fabsf(y) > 1.0f)
        {
            float y_speed = 10.0f;
            float y_direction = y > 0.0f ? 1.0f : -1.0f;
            float y_turn_value = y_direction * y_speed * time;

            scene_node->yaw(Ogre::Degree(y_turn_value));
        }

        if (std::fabsf(p) > 1.0f)
        {
            float p_speed = 10.0f;
            float p_direction = p > 0.0f ? 1.0f : -1.0f;
            float p_turn_value = p_direction * p_speed * time;

            scene_node->pitch(Ogre::Degree(p_turn_value));
        }
    }
}
