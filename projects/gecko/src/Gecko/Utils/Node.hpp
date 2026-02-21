#pragma once

namespace Gecko::Utils::Node
{
    Ogre::Vector3 get_direction(Ogre::SceneNode* scene_node);

    const Ogre::Vector3& get_position(Ogre::SceneNode* scene_node);

    void rotate_towards_position(Ogre::SceneNode* scene_node, const Ogre::Vector3& position, float time);
}
