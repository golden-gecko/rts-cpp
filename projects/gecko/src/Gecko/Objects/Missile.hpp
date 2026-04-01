#pragma once

#include "Gecko/Objects/Object.hpp"

namespace Gecko
{
    class Missile :
        public Object
    {
    private:
        using base_type = Object;

    // From Serializable.
    public:
        ConfigurationPtr serialize() const override;
        void deserialize(const ConfigurationPtr& configuration) override;

    // From Updatable.
    public:
        void update(float time) override;

    // Missile.
    public:
        static Missile* create(Missile* memory, const ConfigurationPtr& configuration, const ScenePtr& scene);

    public:
        explicit Missile(const ScenePtr& scene);

    public:
        void set_position(const Ogre::Vector3& position, bool validate = true) override;

    private:
        float m_damage = 0.0f;
        float m_speed = 0.0f;
    };
}
