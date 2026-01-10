#include "Gecko/Cameras/ObjectFollowCamera.hpp"

#include "Gecko/Input.hpp"
#include "Gecko/Layers/SquareLayer.hpp"
#include "Gecko/Managers/MapManager.hpp"
#include "Gecko/Maps/Map.hpp"

namespace Gecko
{
    ObjectFollowCamera::ObjectFollowCamera(Ogre::Root* root, Ogre::SceneManager* scene_manager, const std::string& name, const ConfigurationPtr& configuration) :
        base_type(root, scene_manager, name, configuration)
    {
    }
}
