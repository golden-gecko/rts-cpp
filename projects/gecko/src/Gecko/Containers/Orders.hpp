#pragma once

#include "Gecko/Interfaces/Serializable.hpp"
#include "Gecko/Orders/Order.hpp"

namespace Gecko
{
    class Orders :
        public Serializable
    {
    public:
        // From Serializable.
        ConfigurationPtr serialize() const override;
        void deserialize(const ConfigurationPtr& configuration) override;

    public:
        typedef std::set<order_type::Value> Available;
        typedef std::deque<Id> Queue;

    public:
        ~Orders() override;

    public:
        void add(const order_type::Value& item)
        {
            m_available.emplace(item);
        }

        bool add_first(Id order_id);
        bool add_last(Id order_id);

        void clear()
        {
            m_queue.clear();
        }

        auto empty() const
        {
            return m_queue.empty();
        }

        const auto& front() const
        {
            return m_queue.front();
        }

        bool is_available(order_type::Value item) const
        {
            return std::ranges::find(m_available, item) != m_available.end();
        }

        void move_first_to_end();

        void remove(Id order_id);
        void remove_current_order();
        void remove_all_orders();

        auto size() const
        {
            return m_queue.size();
        }

    public:
        // TODO: Remove.
        auto& get_available()
        {
            return m_available;
        }

        const auto& get_available() const
        {
            return m_available;
        }

        // TODO: Remove.
        auto& get_queue()
        {
            return m_queue;
        }

        const auto& get_queue() const
        {
            return m_queue;
        }

    public:
        // TODO: Iterate over m_available or m_queue?
        auto begin() const
        {
            return m_available.begin();
        }

        auto end() const
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
        bool operator==(const Orders& other) const
        {
            return m_available == other.m_available && m_queue == other.m_queue;
        }

    private:
        Available m_available;
        Queue m_queue;
    };
}
