#pragma once

#include "Gecko/Components/Mesh.hpp"

namespace Gecko
{
    class Drive :
        public Mesh
    {
    private:
        using base_type = Mesh;

    public:
        static Drive* create(Drive* memory, const ConfigurationPtr& configuration);

    public:
        explicit Drive() = default;
        explicit Drive(const Drive& other);

        ConfigurationPtr serialize() const override;
        void deserialize(const ConfigurationPtr& configuration) override;

    public:
        auto get_distance_driven() const
        {
            return distance_driven;
        }

        auto get_speed() const
        {
            return speed;
        }

    private:
        float speed = 0.0f;
        float distance_driven = 0.0f;
    };
}
