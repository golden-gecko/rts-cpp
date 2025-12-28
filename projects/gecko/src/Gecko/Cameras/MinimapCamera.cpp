#include "Gecko/Cameras/MinimapCamera.hpp"

#include "Gecko/Input.hpp"
#include "Gecko/Layers/SquareLayer.hpp"
#include "Gecko/Managers/MapManager.hpp"
#include "Gecko/Maps/Map.hpp"

namespace Gecko
{
    MinimapCamera::MinimapCamera(Ogre::Root* root, Ogre::SceneManager* scene_manager, const std::string& name, const std::shared_ptr<Configuration>& configuration) :
        base_type(root, scene_manager, name, configuration)
    {
    }
}
