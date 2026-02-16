#pragma once

#include "Gecko/Cameras/Camera.hpp"

namespace Gecko
{
    class FreeCamera :
        public Camera
    {
    private:
        using base_type = Camera;

    public:
        // From Updatable.
        void update(float time) override;

    public:
        explicit FreeCamera(Scene* scene, const std::string& name, const ConfigurationPtr& configuration);
    };
}
