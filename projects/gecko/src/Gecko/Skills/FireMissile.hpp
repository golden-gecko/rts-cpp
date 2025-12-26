#pragma once

#include "Gecko/Skills/Skill.hpp"

namespace Gecko
{
    class FireMissile : public Skill
    {
    private:
        using base_type = Skill;

    public:
        static FireMissile* create(FireMissile* memory, const std::shared_ptr<Configuration>& configuration);

    public:
        explicit FireMissile() = default;

        std::shared_ptr<Configuration> serialize() const override;
        void deserialize(const std::shared_ptr<Configuration>& configuration) override;

    public:
        void activate(const Id& id) override;
        void activate(const Ogre::Vector3& position) override;

    protected:
        std::string m_missile_configuration;
    };
}
