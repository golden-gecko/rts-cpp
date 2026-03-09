#pragma once

#include "Gecko/Objects/Object.hpp"

namespace Gecko
{
    class Missile :
        public Object
    {
    private:
        using base_type = Object;

    public:
        // From Serializable.
        ConfigurationPtr serialize() const override;
        void deserialize(const ConfigurationPtr& configuration) override;

    public:
        // From Updatable.
        void update(float time) override;

    public:
        static Missile* create(Missile* memory, const ConfigurationPtr& configuration, const ScenePtr& scene);

    public:
        explicit Missile(const ScenePtr& scene);
        Missile(const Missile& other);

    public:
        void set_position(const Ogre::Vector3& position, bool validate = true) override;

    private:
        float m_damage = 0.0f;
        float m_speed = 0.0f;
    };
}
