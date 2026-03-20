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
    void SkillManager::init()
    {
        L_TIME("SkillManager::init()");

        std::uint64_t max_size = m_configuration->get_int<std::uint64_t>("memory.skills");

        for (const auto& [name, configuration] : ConfigurationManager::getSingleton())
        {
            L_INFO << "Loading '" << name << "' configuration.";

            if (std::optional<std::string> type = configuration->get_string_optional("type"))
            {
                if (type == TO_STRING(Buff))
                {
                    register_type<Buff>(name, std::bind(Buff::create, std::placeholders::_1, configuration));
                }
                else if (type == TO_STRING(FireMissile))
                {
                    register_type<FireMissile>(name, std::bind(FireMissile::create, std::placeholders::_1, configuration));
                }
                else if (type == TO_STRING(Repair))
                {
                    register_type<Repair>(name, std::bind(Repair::create, std::placeholders::_1, configuration));
                }

                allocate(name, max_size);
            }
        }
    }

    void SkillManager::deinit()
    {
        unregister_all();
    }

    SkillManager::SkillManager(const ConfigurationPtr& configuration) :
        m_configuration(configuration)
    {
    }
}
