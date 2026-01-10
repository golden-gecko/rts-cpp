#pragma once

#include "Gecko/Skills/Skill.hpp"

namespace Gecko
{
    class FireMissile :
        public Skill
    {
    private:
        using base_type = Skill;

    public:
        static FireMissile* create(FireMissile* memory, const ConfigurationPtr& configuration);

    public:
        explicit FireMissile() = default;

        ConfigurationPtr serialize() const override;
        void deserialize(const ConfigurationPtr& configuration) override;

    public:
        void activate(const Id& id) override;
        void activate(const Ogre::Vector3& position) override;

    protected:
        std::string m_missile_configuration;
    };
}
