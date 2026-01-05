#pragma once

#include "Gecko/Components/Mesh.hpp"

#include "Gecko/Timer.hpp"

namespace Gecko
{
    class Weapon :
        public Mesh
    {
    private:
        using base_type = Mesh;

    public:
        static Weapon* create(Weapon* memory, const std::shared_ptr<Configuration>& configuration);

    public:
        explicit Weapon() = default;
        explicit Weapon(const Weapon& other);

        void init() override;

        std::shared_ptr<Configuration> serialize() const override;
        void deserialize(const std::shared_ptr<Configuration>& configuration) override;

        void update(float time) override;

    private:
        std::string missile_configuration;
        std::uint16_t missiles_fired = 0;
        float rotation_speed = 0.0f;
        Timer reload_timer;
    };
}
