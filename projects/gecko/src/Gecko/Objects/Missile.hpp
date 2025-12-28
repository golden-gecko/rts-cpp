#pragma once

#include "Gecko/Objects/Object.hpp"

namespace Gecko
{
    class Missile : public Object
    {
    private:
        using base_type = Object;

    public:
        static Missile* create(Missile* memory, const std::shared_ptr<Configuration>& configuration);

    public:
        explicit Missile() = default;
        explicit Missile(const Missile& other);

        std::shared_ptr<Configuration> serialize() const override;
        void deserialize(const std::shared_ptr<Configuration>& configuration) override;

        void update(float time) override;

    public:
        void set_position(const Ogre::Vector3& position, bool validate = true) override;

    private:
        float damage = 0.0f;
        float speed = 0.0f;
    };
}
