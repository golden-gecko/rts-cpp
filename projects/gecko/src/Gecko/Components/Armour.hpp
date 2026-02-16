#pragma once

#include "Gecko/Components/Component.hpp"

namespace Gecko
{
    class Armour :
        public Component
    {
    private:
        using base_type = Component;

    public:
        // From Serializable.
        ConfigurationPtr serialize() const override;
        void deserialize(const ConfigurationPtr& configuration) override;

    public:
        static Armour* create(Armour* memory, const ConfigurationPtr& configuration, const ScenePtr& scene);

    public:
        explicit Armour() = default;
        explicit Armour(const Armour& other);

    private:
        float m_defence = 0.0f;
    };
}
