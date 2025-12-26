#pragma once

#include "Gecko/Components/Component.hpp"

namespace Gecko
{
    class Armour : public Component
    {
    private:
        using base_type = Component;

    public:
        static Armour* create(Armour* memory, const std::shared_ptr<Configuration>& configuration);

    public:
        explicit Armour() = default;
        explicit Armour(const Armour& other);

        std::shared_ptr<Configuration> serialize() const override;
        void deserialize(const std::shared_ptr<Configuration>& configuration) override;

    private:
        float m_defence = 0.0f;
    };
}
