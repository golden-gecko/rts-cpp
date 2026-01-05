#pragma once

#include "Gecko/Cameras/Camera.hpp"

namespace Gecko
{
    class ObjectFollowCamera :
        public Camera
    {
    private:
        using base_type = Camera;

    public:
        explicit ObjectFollowCamera(Ogre::Root* root, Ogre::SceneManager* scene_manager, const std::string& name, const std::shared_ptr<Configuration>& configuration);
    };
}
