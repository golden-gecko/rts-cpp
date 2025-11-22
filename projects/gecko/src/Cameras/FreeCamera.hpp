#pragma once

#include "Cameras/Camera.hpp"

namespace Gecko
{
    class FreeCamera : public Camera
    {
    private:
        using base_type = Camera;

    public:
        explicit FreeCamera(Ogre::Root* root, Ogre::SceneManager* scene_manager, const std::string& name, const std::shared_ptr<Configuration>& configuration);

        void update(float time) override;
    };
}
