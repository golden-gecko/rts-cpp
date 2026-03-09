#pragma once

#include "Gecko/Interfaces/Serializable.hpp"

namespace Gecko
{
    class Components :
        public Serializable
    {
    public:
        using Items = std::vector<ComponentPtr>;

    public:
        // From Serializable.
        ConfigurationPtr serialize() const override;
        void deserialize(const ConfigurationPtr& configuration) override;

    public:
        explicit Components() = default;
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

        Items::size_type size() const
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
        auto begin()
        {
            return m_items.begin();
        }

        auto end()
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

        void add_component(const std::string& configuration_name, const ConfigurationPtr& overriden_configuration = ConfigurationPtr());
    };
}
