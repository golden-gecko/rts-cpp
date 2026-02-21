#pragma once

#include "Gecko/Id.hpp"

namespace Gecko
{
    using LayerRaycastResults = std::optional<std::pair<Layer*, Ogre::Vector3>>;
    using ObjectRaycastResults = std::optional<std::pair<Id, Ogre::Vector3>>;
}

namespace Gecko::Utils::Raycast
{
    LayerRaycastResults to_layer(const ScenePtr& scene, const OIS::MouseEvent& arg);
    ObjectRaycastResults to_object(const ScenePtr& scene, const OIS::MouseEvent& arg);

    LayerRaycastResults to_layer(const ScenePtr& scene, const std::optional<Ogre::Ray>& ray);
    ObjectRaycastResults to_object(const ScenePtr& scene, const std::optional<Ogre::Ray>& ray);

    std::set<Id> to_objects(const ScenePtr& scene, const Ogre::Vector2& start, const Ogre::Vector2& end);
}
