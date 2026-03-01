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

        Size max_size = m_configuration->get_int<Size>("memory.skills");

        for (const auto& [name, configuration] : ConfigurationManager::getSingleton())
        {
            L_INFO << "Loading '" << name << "' configuration.";

            std::string type = configuration->get_string("type", "");

            if (type == "Buff")
            {
                register_type<Buff>(name, std::bind(Buff::create, std::placeholders::_1, configuration));
            }
            else if (type == "FireMissile")
            {
                register_type<FireMissile>(name, std::bind(FireMissile::create, std::placeholders::_1, configuration));
            }
            else if (type == "Repair")
            {
                register_type<Repair>(name, std::bind(Repair::create, std::placeholders::_1, configuration));
            }

            allocate(name, max_size);
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
