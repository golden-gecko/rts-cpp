#pragma once

#include "Gecko/Skills/Skill.hpp"

namespace Gecko
{
    class Repair :
        public Skill
    {
    private:
        using base_type = Skill;

    public:
        static Repair* create(Repair* memory, const ConfigurationPtr& configuration);

    public:
        explicit Repair() = default;

        ConfigurationPtr serialize() const override;
        void deserialize(const ConfigurationPtr& configuration) override;

    public:
        void activate(const Id& id) override;
        void activate(const Ogre::Vector3& position) override;

    private:
        float m_percentage = 0.0f;
        float m_value = 0.0f;
    };
}
