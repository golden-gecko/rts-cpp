#pragma once

#include "Interfaces/Serializable.hpp"

namespace Gecko
{
    class Components : public Serializable
    {
    public:
        typedef std::vector<Component*> Items;

    public:
        explicit Components() = default;
        explicit Components(const Components& other);

        std::shared_ptr<Configuration> serialize() const override;
        void deserialize(const std::shared_ptr<Configuration>& configuration) override;

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

        void add_component(const std::string& configuration, const std::shared_ptr<Configuration>& component_configuration);
    };
}
