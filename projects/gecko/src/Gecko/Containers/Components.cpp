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
            ConfigurationPtr component_configuration = std::make_shared<Configuration>(i);
            std::string configuration = component_configuration->get_string("configuration");

            add_component(configuration, component_configuration);
        }

        // TODO: Inject debug information in outer scope.
        add_component("debug");
    }

    void Components::add_component(const std::string& configuration_name, const ConfigurationPtr& overriden_configuration)
    {
        ComponentPtr component = ComponentManager::getSingleton().create(configuration_name);

        if (component == nullptr)
        {
            throw Exception("Failed to create '" + configuration_name + "' component.");
        }

        ConfigurationPtr configuration = ConfigurationManager::getSingleton().get(configuration_name);
        ConfigurationPtr merged_configuration = std::make_shared<Configuration>(*configuration.get());

        if (overriden_configuration)
        {
            merged_configuration->merge(overriden_configuration);
        }

        component->set_configuration(merged_configuration);

        m_items.emplace_back(component);
    }
}
