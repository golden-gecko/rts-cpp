#pragma once

#include "Gecko/Id.hpp"

namespace Gecko::Utils::Raycast
{
    std::optional<std::pair<Layer*, Ogre::Vector3>> to_layer(const ScenePtr& scene, const OIS::MouseEvent& arg);
    std::optional<std::pair<Id, Ogre::Vector3>> to_object(const ScenePtr& scene, const OIS::MouseEvent& arg);

    std::optional<std::pair<Layer*, Ogre::Vector3>> to_layer(const ScenePtr& scene, const Ogre::Ray& ray);
    std::optional<std::pair<Id, Ogre::Vector3>> to_object(const ScenePtr& scene, const Ogre::Ray& ray);

    std::set<Id> to_objects(const ScenePtr& scene, const Ogre::Vector2& start, const Ogre::Vector2& end);
}
