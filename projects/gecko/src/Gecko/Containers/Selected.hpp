#pragma once

#include "Gecko/Interfaces/Serializable.hpp"

namespace Gecko
{
    class Selected :
        public Serializable
    {
    public:
        // From Serializable.
        ConfigurationPtr serialize() const override;
        void deserialize(const ConfigurationPtr& configuration) override;

    public:
        typedef std::set<Id> Items;
        typedef std::map<std::uint16_t, Items> Groups;

    public:
        void apply_current_selection(bool select);
        void apply_ui(bool select);

        void clear()
        {
            apply_selection(m_items, false);

            m_items.clear();
        }

        void create_group(std::uint16_t group_number);

        void select(Id object_id, bool add = false);
        void select(const Items& object_ids, bool add = false);
        void select_group(std::uint16_t group_number);

        auto empty() const
        {
            return m_items.empty();
        }

        auto size() const
        {
            return m_items.size();
        }

    public:
        auto begin()
        {
            return m_items.begin();
        }

        const auto begin() const
        {
            return m_items.cbegin();
        }

        auto end()
        {
            return m_items.end();
        }

        const auto end() const
        {
            return m_items.cend();
        }

    public:
        bool operator==(const Selected& other) const
        {
            return m_items == other.m_items && groups == other.groups;
        }

    private:
        Items m_items;
        Groups groups;

        void apply_selection(const Items& object_ids, bool select);
    };
}
