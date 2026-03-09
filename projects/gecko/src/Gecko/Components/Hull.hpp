#pragma once

#include "Gecko/Components/Mesh.hpp"

namespace Gecko
{
    class Hull :
        public Mesh
    {
    private:
        using base_type = Mesh;

    public:
        static Hull* create(Hull* memory, const ConfigurationPtr& configuration, const ScenePtr& scene);

    public:
        explicit Hull() = default;
        Hull(const Hull& other) = default;
    };
}
