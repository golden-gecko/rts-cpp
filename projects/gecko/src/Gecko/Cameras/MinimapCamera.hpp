#pragma once

#include "Gecko/Cameras/Camera.hpp"

namespace Gecko
{
    class MinimapCamera :
        public Camera
    {
    private:
        using base_type = Camera;

    public:
        explicit MinimapCamera(const std::string& name, const ConfigurationPtr& configuration, Scene* scene);
    };
}
