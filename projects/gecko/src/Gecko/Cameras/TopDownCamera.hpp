#pragma once

#include "Gecko/Cameras/Camera.hpp"

namespace Gecko
{
    class TopDownCamera :
        public Camera
    {
    private:
        using base_type = Camera;

    public:
        // From Camera.
        void update(float time) override;

    public:
        explicit TopDownCamera(Ogre::Root* root, Ogre::SceneManager* scene_manager, const std::string& name, const ConfigurationPtr& configuration);
    };
}
