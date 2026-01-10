#pragma once

#include "Gecko/Skills/Skill.hpp"

namespace Gecko
{
    class Buff :
        public Skill
    {
    private:
        using base_type = Skill;

    public:
        static Buff* create(Buff* memory, const ConfigurationPtr& configuration);

    public:
        explicit Buff() = default;

        ConfigurationPtr serialize() const override;
        void deserialize(const ConfigurationPtr& configuration) override;

    public:
        void activate(const Id& id) override;
        void activate(const Ogre::Vector3& position) override;
    };
}
