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
        // From Serializable.
        ConfigurationPtr serialize() const override;
        void deserialize(const ConfigurationPtr& configuration) override;

    public:
        // From Skill.
        void activate(const Id& id) override;
        void activate(const Ogre::Vector3& position) override;

    public:
        static Buff* create(Buff* memory, const ConfigurationPtr& configuration);

    public:
        explicit Buff() = default;
    };
}
