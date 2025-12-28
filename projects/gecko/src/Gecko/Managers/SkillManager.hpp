#pragma once

#include "Gecko/Id.hpp"
#include "Gecko/Managers/Manager.hpp"

namespace Gecko
{
    class SkillManager :
        public Ogre::Singleton<SkillManager>,
        public Manager<Skill, std::string, Id>
    {
    private:
        using base_type = Manager<Skill, std::string, Id>;
    };
}
