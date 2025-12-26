#pragma once

#include "Gecko/Skills/Skill.hpp"

namespace Gecko
{
    class Repair : public Skill
    {
    private:
        using base_type = Skill;

    public:
        static Repair* create(Repair* memory, const std::shared_ptr<Configuration>& configuration);

    public:
        explicit Repair() = default;

        std::shared_ptr<Configuration> serialize() const override;
        void deserialize(const std::shared_ptr<Configuration>& configuration) override;

    public:
        void activate(const Id& id) override;
        void activate(const Ogre::Vector3& position) override;

    private:
        float m_percentage = 0.0f;
        float m_value = 0.0f;
    };
}
