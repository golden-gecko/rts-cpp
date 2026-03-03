#pragma once

#include "Gecko/Components/Component.hpp"

namespace Gecko
{
    class Weapon :
        public Component
    {
    private:
        using base_type = Component;

    public:
        // From Initializable.
        void init() override;

    public:
        // From Serializable.
        ConfigurationPtr serialize() const override;
        void deserialize(const ConfigurationPtr& configuration) override;

    public:
        // From Updatable.
        void update(float time) override;

    public:
        static Weapon* create(Weapon* memory, const ConfigurationPtr& configuration, const ScenePtr& scene);

    public:
        explicit Weapon() = default;
        explicit Weapon(const Weapon& other);

    private:
        std::string m_missile_configuration;
        std::uint16_t m_missiles_fired = 0;
        float m_rotation_speed = 0.0f;
        Timer m_reload_timer;
    };
}
