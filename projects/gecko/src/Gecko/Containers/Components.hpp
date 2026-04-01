#pragma once

#include "Gecko/Interfaces/Serializable.hpp"

namespace Gecko
{
    class Components :
        public Serializable
    {
    public:
        using Container = std::vector<ComponentPtr>;

    // From Serializable.
    public:
        ConfigurationPtr serialize() const override;
        void deserialize(const ConfigurationPtr& configuration) override;

    // Components.
    public:
        Components() = default;
        Components(const Components& other);

    public:
        void clear()
        {
            m_items.clear();
        }

        bool empty() const
        {
            return m_items.empty();
        }

        template<typename Type>
        bool has() const
        {
            for (const auto i : m_items)
            {
                if (dynamic_cast<Type*>(i) != nullptr)
                {
                    return true;
                }
            }

            return false;
        }

        Container::size_type size() const
        {
            return m_items.size();
        }

    public:
        template<typename Type>
        std::vector<Type*> get_by_type() const
        {
            std::vector<Type*> items;

            for (const auto i : m_items)
            {
                auto j = dynamic_cast<Type*>(i);

                if (j != nullptr)
                {
                    items.push_back(j);
                }
            }

            return items;
        }

    public:
        Container::iterator begin()
        {
            return m_items.begin();
        }

        Container::iterator end()
        {
            return m_items.end();
        }

        Container::const_iterator cbegin() const
        {
            return m_items.cbegin();
        }

        Container::const_iterator cend() const
        {
            return m_items.cend();
        }

    private:
        Container m_items;

        void add_component(const std::string& configuration_name, const ConfigurationPtr& overriden_configuration = ConfigurationPtr());
    };
}
