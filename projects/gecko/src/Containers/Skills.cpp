#include "Containers/Skills.hpp"

#include "Configuration.hpp"
#include "Exception.hpp"
#include "Managers/ConfigurationManager.hpp"
#include "Managers/SkillManager.hpp"
#include "Skills/Skill.hpp"

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

    std::shared_ptr<Configuration> Skills::serialize() const
    {
        auto configuration = std::make_shared<Configuration>();

        for (const auto& i : m_items)
        {
            configuration->append(i->serialize());
        }

        return configuration;
    }

    void Skills::deserialize(const std::shared_ptr<Configuration>& configuration)
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
        // TODO: ID should be passed here, because object may have more than one skill?
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
        // TODO: ID should be passed here, because object may have more than one skill?
        for (const auto& i : m_items)
        {
            if (i->get_configuration()->get_name() == name)
            {
                i->activate(position);

                break;
            }
        }
    }
}
