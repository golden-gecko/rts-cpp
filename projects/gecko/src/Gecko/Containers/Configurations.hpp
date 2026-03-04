#pragma once

#include "Gecko/Interfaces/Serializable.hpp"

namespace Gecko
{
    class Configurations :
        public Serializable
    {
    public:
        using Available = std::set<std::string>;

    public:
        // From Serializable.
        ConfigurationPtr serialize() const override;
        void deserialize(const ConfigurationPtr& configuration) override;

    public:
        void add(const std::string& item)
        {
            m_available.emplace(item);
        }

        void clear()
        {
            m_available.clear();
        }

        auto empty() const
        {
            return m_available.empty();
        }

        bool is_available(const std::string& item) const
        {
            return std::ranges::find(m_available, item) != m_available.end();
        }

        auto size() const
        {
            return m_available.size();
        }

    public:
        auto begin()
        {
            return m_available.begin();
        }

        auto end()
        {
            return m_available.end();
        }

        auto cbegin() const
        {
            return m_available.cbegin();
        }

        auto cend() const
        {
            return m_available.cend();
        }

    public:
        bool operator==(const Configurations& other) const
        {
            return m_available == other.m_available;
        }

    private:
        Available m_available;
    };
}
