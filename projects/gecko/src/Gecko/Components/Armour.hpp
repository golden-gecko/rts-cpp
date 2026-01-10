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
        static Armour* create(Armour* memory, const ConfigurationPtr& configuration);

    public:
        explicit Armour() = default;
        explicit Armour(const Armour& other);

        ConfigurationPtr serialize() const override;
        void deserialize(const ConfigurationPtr& configuration) override;

    private:
        float m_defence = 0.0f;
    };
}
