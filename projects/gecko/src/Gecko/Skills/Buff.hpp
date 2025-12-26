#pragma once

#include "Gecko/Skills/Skill.hpp"

namespace Gecko
{
    class Buff : public Skill
    {
    private:
        using base_type = Skill;

    public:
        static Buff* create(Buff* memory, const std::shared_ptr<Configuration>& configuration);

    public:
        explicit Buff() = default;

        std::shared_ptr<Configuration> serialize() const override;
        void deserialize(const std::shared_ptr<Configuration>& configuration) override;

    public:
        void activate(const Id& id) override;
        void activate(const Ogre::Vector3& position) override;
    };
}
