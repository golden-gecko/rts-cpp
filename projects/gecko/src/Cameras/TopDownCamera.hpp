#pragma once

#include "Cameras/Camera.hpp"

namespace Gecko
{
    class TopDownCamera : public Camera
    {
    private:
        using base_type = Camera;

    public:
        explicit TopDownCamera(Ogre::Root* root, Ogre::SceneManager* scene_manager, const std::string& name, const std::shared_ptr<Configuration>& configuration);

        void update(float time) override;
    };
}
