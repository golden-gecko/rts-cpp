#include "Gecko/Managers/SkillManager.hpp"

#include "Gecko/Configuration.hpp"
#include "Gecko/Log.hpp"
#include "Gecko/Managers/ConfigurationManager.hpp"
#include "Gecko/Skills/Buff.hpp"
#include "Gecko/Skills/FireMissile.hpp"
#include "Gecko/Skills/Repair.hpp"
#include "Gecko/Utils/Time.hpp"

Gecko::SkillManager* Ogre::Singleton<Gecko::SkillManager>::msSingleton = nullptr;

namespace Gecko
{
    void SkillManager::init(const ConfigurationPtr& configuration)
    {
        L_TIME("SkillManager::init()");

        auto max_size = configuration->get_int<std::size_t>("memory.skills");
        auto& skill_manager = SkillManager::getSingleton();

        for (const auto& [name, configuration] : ConfigurationManager::getSingleton())
        {
            L_INFO << "Loading '" << name << "' configuration.";

            auto type = configuration->get_string("type", "");

            if (type == "Buff")
            {
                auto factory = std::bind(Buff::create, std::placeholders::_1, configuration);

                skill_manager.register_type<Buff>(name, factory);
                skill_manager.allocate(name, max_size);
            }
            else if (type == "FireMissile")
            {
                auto factory = std::bind(FireMissile::create, std::placeholders::_1, configuration);

                skill_manager.register_type<FireMissile>(name, factory);
                skill_manager.allocate(name, max_size);
            }
            else if (type == "Repair")
            {
                auto factory = std::bind(Repair::create, std::placeholders::_1, configuration);

                skill_manager.register_type<Repair>(name, factory);
                skill_manager.allocate(name, max_size);
            }
        }
    }

    void SkillManager::deinit()
    {
        deallocate();
    }
}
