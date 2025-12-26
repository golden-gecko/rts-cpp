#pragma once

#include "Gecko/Components/Mesh.hpp"

namespace Gecko
{
    class Drive : public Mesh
    {
    private:
        using base_type = Mesh;

    public:
        static Drive* create(Drive* memory, const std::shared_ptr<Configuration>& configuration);

    public:
        explicit Drive() = default;
        explicit Drive(const Drive& other);

        std::shared_ptr<Configuration> serialize() const override;
        void deserialize(const std::shared_ptr<Configuration>& configuration) override;

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
