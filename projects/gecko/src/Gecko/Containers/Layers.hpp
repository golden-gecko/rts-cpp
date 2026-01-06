#pragma once

#include "Gecko/Interfaces/Serializable.hpp"

namespace Gecko
{
    class Layers :
        public Serializable
    {
    public:
        // From Serializable.
        std::shared_ptr<Configuration> serialize() const override;
        void deserialize(const std::shared_ptr<Configuration>& configuration) override;

    public:
        typedef std::set<std::string> Items;

    public:
        void clear()
        {
            m_items.clear();
        }

        auto empty() const
        {
            return m_items.empty();
        }

        bool has(const std::string& item) const
        {
            return m_items.find(item) == m_items.end();
        }

        auto size() const
        {
            return m_items.size();
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

    public:
        bool operator==(const Layers& other) const
        {
            return m_items == other.m_items;
        }

    private:
        Items m_items;
    };
}
