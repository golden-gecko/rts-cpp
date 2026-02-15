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
        explicit TopDownCamera(Scene* scene, const std::string& name, const ConfigurationPtr& configuration);
    };
}
