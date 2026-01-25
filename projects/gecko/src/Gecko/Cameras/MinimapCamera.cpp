#include "Gecko/Cameras/MinimapCamera.hpp"

namespace Gecko
{
    MinimapCamera::MinimapCamera(Ogre::Root* root, Ogre::SceneManager* scene_manager, const std::string& name, const ConfigurationPtr& configuration) :
        base_type(root, scene_manager, name, configuration)
    {
    }
}
