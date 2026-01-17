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
        std::string m_missile_configuration;
        std::uint16_t m_missiles_fired = 0;
        float m_rotation_speed = 0.0f;
        Timer m_reload_timer;
    };
}
