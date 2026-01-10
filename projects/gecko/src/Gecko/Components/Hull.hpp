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
        static Hull* create(Hull* memory, const ConfigurationPtr& configuration);

    public:
        explicit Hull() = default;
        explicit Hull(const Hull& other) = default;
    };
}
