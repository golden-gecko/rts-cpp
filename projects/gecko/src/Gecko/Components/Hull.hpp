#pragma once

#include "Gecko/Components/Mesh.hpp"

namespace Gecko
{
    class Hull : public Mesh
    {
    private:
        using base_type = Mesh;

    public:
        static Hull* create(Hull* memory, const std::shared_ptr<Configuration>& configuration);

    public:
        explicit Hull() = default;
        explicit Hull(const Hull& other) = default;
    };
}
