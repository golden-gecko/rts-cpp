#pragma once

#include "Gecko/Cameras/Camera.hpp"

namespace Gecko
{
    class TopDownCamera :
        public Camera
    {
    private:
        using base_type = Camera;

    // From Camera.
    public:
        void update(float time) override;

    // TopDownCamera.
    public:
        explicit TopDownCamera(const std::string& name, const ConfigurationPtr& configuration, Scene* scene);
    };
}
