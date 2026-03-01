#pragma once

#include "Gecko/Id.hpp"
#include "Gecko/Interfaces/Initializable.hpp"
#include "Gecko/Managers/Manager.hpp"

namespace Gecko
{
    class SkillManager :
        public Ogre::Singleton<SkillManager>,
        public Manager<Skill, std::string, Id>
    {
    private:
        using base_type = Manager<Skill, std::string, Id>;

    public:
        // From Initializable.
        void init();
        void deinit();
    
    public:
        SkillManager(const ConfigurationPtr& configuration);

    private:
        ConfigurationPtr m_configuration;
    };
}
