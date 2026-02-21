#include "Gecko/Containers/Resources.hpp"

#include "Gecko/Configuration.hpp"

namespace Gecko
{
    ConfigurationPtr Resources::serialize() const
    {
        ConfigurationPtr configuration = std::make_shared<Configuration>();

        for (const auto& [_, resource] : m_items)
        {
            configuration->append("items", resource.serialize());
        }

        configuration->set("max_storage", max_storage);

        return configuration;
    }

    void Resources::deserialize(const ConfigurationPtr& configuration)
    {
        clear();

        if (configuration->has_member("items"))
        {
            ConfigurationPtr child = configuration->get_child("items");

            for (auto i = child->begin(); i != child->end(); i++)
            {
                auto name = i.key().asString();
                auto resource = Resource(name);

                resource.deserialize(std::make_shared<Configuration>(*i));

                m_items.emplace(name, resource);
            }
        }

        if (configuration->has_member("max_storage"))
        {
            max_storage = configuration->get_float("max_storage");
        }
        else
        {
            max_storage = 0.0f;

            for (const auto& i : m_items)
            {
                max_storage += i.second.get_max();
            }
        }
    }

    void Resources::update(float time)
    {
        for (auto& [_, resource] : m_items)
        {
            resource.update(time);
        }
    }

    float Resources::add(const std::string& name, float value)
    {
        Container::iterator resource = m_items.find(name);

        if (resource == m_items.end())
        {
            return 0.0f;
        }

        return resource->second.add(value);
    }

    bool Resources::has_resource(const std::string& name) const
    {
        return m_items.find(name) != m_items.end();
    }

    float Resources::get_current(const std::string& name) const
    {
        Container::const_iterator resource = m_items.find(name);

        if (resource == m_items.end())
        {
            return 0.0f;
        }

        return resource->second.get_current();
    }

    float Resources::get_storage(const std::string& name) const
    {
        Container::const_iterator resource = m_items.find(name);

        if (resource == m_items.end())
        {
            return 0.0f;
        }

        float current = 0.0f;

        for (const auto& i : get_items())
        {
            current += i.second.get_current();
        }

        return std::max(std::min(max_storage - current, resource->second.get_storage()), 0.0f);
    }

    bool Resources::has_resource(const std::string& name, float value) const
    {
        Container::const_iterator resource = m_items.find(name);

        if (resource == m_items.end())
        {
            return false;
        }

        return resource->second.get_current() >= value;
    }

    bool Resources::has_storage(const std::string& name) const
    {
        return m_items.find(name) != m_items.end();
    }

    bool Resources::has_storage(const std::string& name, float value) const
    {
        Container::const_iterator resource = m_items.find(name);

        if (resource == m_items.end())
        {
            return false;
        }

        return resource->second.get_max() - resource->second.get_current() >= value;
    }

    void Resources::merge(const Resources& other)
    {
        for (auto& [other_name, other_resource] : other.m_items)
        {
            if (has_resource(other_name) == false)
            {
                m_items.emplace(other_name, Resource(other_name));
            }

            Resource& resource = get(other_name);

            resource.set_consumption(resource.get_consumption() + other_resource.get_consumption());
            resource.set_production(resource.get_production() + other_resource.get_production());
            resource.set_current(resource.get_current() + other_resource.get_current());
            resource.set_max(resource.get_max() + other_resource.get_max());
        }
    }

    float Resources::remove(const std::string& name, float value)
    {
        Container::iterator resource = m_items.find(name);

        if (resource == m_items.end())
        {
            return 0.0f;
        }

        return resource->second.remove(value);
    }

    bool Resources::operator==(const Resources& other) const
    {
        static constexpr auto epsilon = std::numeric_limits<float>::epsilon();

        return std::abs(max_storage - other.max_storage) < epsilon && m_items == other.m_items;
    }
}
