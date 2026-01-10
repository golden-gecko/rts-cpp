#pragma once

#include "Gecko/Components/Mesh.hpp"

namespace Gecko
{
    class Weapon :
        public Mesh
    {
    private:
        using base_type = Mesh;

    public:
        static Weapon* create(Weapon* memory, const ConfigurationPtr& configuration);

    public:
        explicit Weapon() = default;
        explicit Weapon(const Weapon& other);

        void init() override;

        ConfigurationPtr serialize() const override;
        void deserialize(const ConfigurationPtr& configuration) override;

        void update(float time) override;

    private:
        std::string missile_configuration;
        std::uint16_t missiles_fired = 0;
        float rotation_speed = 0.0f;
        Timer reload_timer;
    };
}
