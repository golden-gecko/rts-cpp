#include "Containers/Components.hpp"

#include "Components/Component.hpp"
#include "Configuration.hpp"
#include "Exception.hpp"
#include "Managers/ComponentManager.hpp"
#include "Managers/ConfigurationManager.hpp"

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

    std::shared_ptr<Configuration> Components::serialize() const
    {
        auto configuration = std::make_shared<Configuration>();

        for (const auto& i : m_items)
        {
            configuration->append(i->serialize());
        }

        return configuration;
    }

    void Components::deserialize(const std::shared_ptr<Configuration>& configuration)
    {
        clear();

        for (const auto& i : *(configuration))
        {
            auto component_configuration = std::make_shared<Configuration>(i);
            auto configuration = component_configuration->get_string("configuration");

            add_component(configuration, component_configuration);
        }

        add_component("debug", std::shared_ptr<Configuration>());
    }

    void Components::add_component(const std::string& configuration, const std::shared_ptr<Configuration>& component_configuration)
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
