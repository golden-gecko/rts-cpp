#include "Gecko/Containers/Skills.hpp"

#include "Gecko/Configuration.hpp"
#include "Gecko/Exception.hpp"
#include "Gecko/Managers/ConfigurationManager.hpp"
#include "Gecko/Managers/SkillManager.hpp"
#include "Gecko/Skills/Skill.hpp"

namespace Gecko
{
    Skills::Skills(const Skills& other)
    {
        for (const auto& i : other.m_items)
        {
            auto configuration = i->get_configuration()->get_name();
            auto skill = SkillManager::getSingleton().create(configuration);

            if (skill == nullptr)
            {
                throw Exception("Failed to create '" + configuration + "' skill.");
            }

            skill->set_configuration(i->get_configuration());
            // TODO: Move to initialize?
            // skill->set_owner(this);

            m_items.emplace_back(skill);
        }
    }

    ConfigurationPtr Skills::serialize() const
    {
        auto configuration = std::make_shared<Configuration>();

        for (const auto& i : m_items)
        {
            configuration->append(i->serialize());
        }

        return configuration;
    }

    void Skills::deserialize(const ConfigurationPtr& configuration)
    {
        clear();

        for (const auto& i : *(configuration))
        {
            auto component_configuration = std::make_shared<Configuration>(i);
            auto configuration = component_configuration->get_string("configuration");
            auto skill = SkillManager::getSingleton().create(configuration);

            if (skill == nullptr)
            {
                throw Exception("Failed to create '" + configuration + "' skill.");
            }

            // TODO: Refactor.
            auto c = ConfigurationManager::getSingleton().get(configuration);
            auto new_configuration = std::make_shared<Configuration>(*c.get());
            new_configuration->merge(component_configuration);

            skill->set_configuration(new_configuration);

            m_items.emplace_back(skill);
        }
    }

    void Skills::activate(const std::string& name, const Id& id)
    {
        for (const auto& i : m_items)
        {
            if (i->get_configuration()->get_name() == name)
            {
                i->activate(id);

                break;
            }
        }
    }

    void Skills::activate(const std::string& name, const Ogre::Vector3& position)
    {
        auto skill = std::find_if(m_items.begin(), m_items.end(), [name](Skill* i) {
            return i->get_configuration()->get_name() == name;
        });

        if (skill != m_items.end())
        {
            (*skill)->activate(position);
        }
    }

    bool Skills::is_available(const std::string& name) const
    {
        for (const auto& i : m_items)
        {
            if (i->get_name() == name)
            {
                return i->is_available();
            }
        }

        return false;
    }
}
