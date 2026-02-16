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
        // From Serializable.
        ConfigurationPtr serialize() const override;
        void deserialize(const ConfigurationPtr& configuration) override;

    public:
        static Drive* create(Drive* memory, const ConfigurationPtr& configuration, const ScenePtr& scene);

    public:
        explicit Drive() = default;
        explicit Drive(const Drive& other);

    public:
        auto get_distance_driven() const
        {
            return m_distance_driven;
        }

        auto get_speed() const
        {
            return m_speed;
        }

    private:
        float m_speed = 0.0f;
        float m_distance_driven = 0.0f;
    };
}
