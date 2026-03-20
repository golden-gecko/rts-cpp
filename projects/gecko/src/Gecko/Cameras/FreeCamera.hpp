#pragma once

#include "Gecko/Cameras/Camera.hpp"

namespace Gecko
{
    class FreeCamera :
        public Camera
    {
    private:
        using base_type = Camera;

    // From Updatable.
    public:
        void update(float time) override;

    // FreeCamera.
    public:
        explicit FreeCamera(const std::string& name, const ConfigurationPtr& configuration, Scene* scene);
    };
}
