#pragma once

#include "Gecko/Interfaces/Serializable.hpp"

namespace Gecko
{
    class Diplomacy :
        public Serializable
    {
    public:
        // From Serializable.
        std::shared_ptr<Configuration> serialize() const override;
        void deserialize(const std::shared_ptr<Configuration>& configuration) override;

    public:
        enum class State :
            std::int8_t
        {
            Ally,
            Enemy
        };

        static State from_name(const std::string& name);

    /*
    public:
        void clear()
        {
            m_items.clear();
        }

        auto empty() const
        {
            return m_items.empty();
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

        auto cend()
        {
            return m_items.cend();
        }
    */
    };
}
