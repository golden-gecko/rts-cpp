#include "Cameras/MinimapCamera.hpp"

#include "Input.hpp"
#include "Layers/SquareLayer.hpp"
#include "Managers/MapManager.hpp"
#include "Maps/Map.hpp"

namespace Gecko
{
    MinimapCamera::MinimapCamera(Ogre::Root* root, Ogre::SceneManager* scene_manager, const std::string& name, const std::shared_ptr<Configuration>& configuration) :
        base_type(root, scene_manager, name, configuration)
    {
    }
}
