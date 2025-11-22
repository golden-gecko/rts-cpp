#include "Cameras/ObjectFollowCamera.hpp"

#include "Input.hpp"
#include "Layers/SquareLayer.hpp"
#include "Managers/MapManager.hpp"
#include "Maps/Map.hpp"

namespace Gecko
{
    ObjectFollowCamera::ObjectFollowCamera(Ogre::Root* root, Ogre::SceneManager* scene_manager, const std::string& name, const std::shared_ptr<Configuration>& configuration) :
        base_type(root, scene_manager, name, configuration)
    {
    }
}
