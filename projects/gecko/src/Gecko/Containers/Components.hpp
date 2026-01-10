#pragma once

#include "Gecko/Interfaces/Serializable.hpp"

namespace Gecko
{
    class Components :
        public Serializable
    {
    public:
        // From Serializable.
        ConfigurationPtr serialize() const override;
        void deserialize(const ConfigurationPtr& configuration) override;

    public:
        typedef std::vector<Component*> Items;

    public:
        explicit Components() = default;
        explicit Components(const Components& other);

    public:
        void clear()
        {
            m_items.clear();
        }

        auto empty() const
        {
            return m_items.empty();
        }

        template<typename Type>
        bool has() const
        {
            for (const auto& i : m_items)
            {
                if (dynamic_cast<Type*>(i) != nullptr)
                {
                    return true;
                }
            }

            return false;
        }

        auto size() const
        {
            return m_items.size();
        }

    public:
        template<typename Type>
        Items get_by_type() const
        {
            Items items;

            for (const auto& i : m_items)
            {
                if (dynamic_cast<Type*>(i))
                {
                    items.emplace_back(i);
                }
            }

            return items;
        }

    public:
        auto begin() const
        {
            return m_items.begin();
        }

        auto end() const
        {
            return m_items.end();
        }

        auto cbegin() const
        {
            return m_items.cbegin();
        }

        auto cend() const
        {
            return m_items.cend();
        }

    private:
        Items m_items;

        void add_component(const std::string& configuration, const ConfigurationPtr& component_configuration);
    };
}
