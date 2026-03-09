#pragma once

#include "Gecko/Components/Component.hpp"

namespace Gecko
{
    class Drive :
        public Component
    {
    private:
        using base_type = Component;

    public:
        // From Serializable.
        ConfigurationPtr serialize() const override;
        void deserialize(const ConfigurationPtr& configuration) override;

    public:
        static Drive* create(Drive* memory, const ConfigurationPtr& configuration, const ScenePtr& scene);

    public:
        explicit Drive() = default;
        Drive(const Drive& other);

    public:
        float get_distance_driven() const
        {
            return m_distance_driven;
        }

        float get_speed() const
        {
            return m_speed;
        }

    private:
        float m_speed = 0.0f;
        float m_distance_driven = 0.0f;
    };
}
