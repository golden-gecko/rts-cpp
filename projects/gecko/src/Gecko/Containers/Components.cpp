#include "Gecko/Containers/Components.hpp"

#include "Gecko/Components/Component.hpp"
#include "Gecko/Configuration.hpp"
#include "Gecko/Exception.hpp"
#include "Gecko/Managers/ComponentManager.hpp"
#include "Gecko/Managers/ConfigurationManager.hpp"

namespace Gecko
{
    Components::Components(const Components& other)
    {
        for (const auto& i : other.m_items)
        {
            auto configuration = i->get_configuration()->get_name();
            auto component = ComponentManager::getSingleton().create(configuration);

            if (component == nullptr)
            {
                throw Exception("Failed to create '" + configuration + "' component.");
            }

            component->set_configuration(i->get_configuration());
            // TODO: Move to initialize?
            // component->set_owner(this);

            m_items.emplace_back(component);
        }
    }

    ConfigurationPtr Components::serialize() const
    {
        auto configuration = std::make_shared<Configuration>();

        for (const auto& i : m_items)
        {
            configuration->append(i->serialize());
        }

        return configuration;
    }

    void Components::deserialize(const ConfigurationPtr& configuration)
    {
        clear();

        for (const auto& i : *(configuration))
        {
            auto component_configuration = std::make_shared<Configuration>(i);
            auto configuration = component_configuration->get_string("configuration");

            add_component(configuration, component_configuration);
        }

        add_component("debug", ConfigurationPtr());
    }

    void Components::add_component(const std::string& configuration, const ConfigurationPtr& component_configuration)
    {
        auto component = ComponentManager::getSingleton().create(configuration);

        if (component == nullptr)
        {
            throw Exception("Failed to create '" + configuration + "' component.");
        }

        // TODO: Refactor.
        auto c = ConfigurationManager::getSingleton().get(configuration);
        auto new_configuration = std::make_shared<Configuration>(*c.get());

        if (component_configuration)
        {
            new_configuration->merge(component_configuration);
        }

        component->set_configuration(new_configuration);

        m_items.emplace_back(component);
    }
}
