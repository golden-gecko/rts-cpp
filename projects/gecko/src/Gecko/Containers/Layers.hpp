#pragma once

#include "Gecko/Interfaces/Serializable.hpp"

namespace Gecko
{
    class Layers :
        public Serializable
    {
    public:
        using Items = std::set<std::string>;
        
    public:
        // From Serializable.
        ConfigurationPtr serialize() const override;
        void deserialize(const ConfigurationPtr& configuration) override;

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

        Items::size_type size() const
        {
            return m_items.size();
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

    public:
        bool operator==(const Layers& other) const
        {
            return m_items == other.m_items;
        }

    private:
        Items m_items;
    };
}
