#pragma once

#include "Gecko/Interfaces/Serializable.hpp"
#include "Gecko/Orders/Order.hpp"

namespace Gecko
{
    class Orders :
        public Serializable
    {
    public:
        using Available = std::set<order_type::Value>;
        using Queue     = std::deque<Id>;

    public:
        // From Serializable.
        ConfigurationPtr serialize() const override;
        void deserialize(const ConfigurationPtr& configuration) override;

    public:
        ~Orders() override;

    public:
        void add_available(const order_type::Value& item);

        bool add_first(const Id& order_id);
        bool add_last(const Id& order_id);

        void clear()
        {
            m_queue.clear();
        }

        bool empty() const
        {
            return m_queue.empty();
        }

        void move_first_to_end();

        void remove(const Id& order_id);
        void remove_current();
        void remove_all_orders();

        Queue::size_type size() const
        {
            return m_queue.size();
        }

    public:
        const Available& get_available() const
        {
            return m_available;
        }
        
        const Id& get_curret() const
        {
            return m_queue.front();
        }

        const Queue& get_queue() const
        {
            return m_queue;
        }

    public:
        bool is_available(order_type::Value item) const
        {
            return std::ranges::find(m_available, item) != m_available.end();
        }

    public:
        Available::iterator begin()
        {
            return m_available.begin();
        }

        Available::iterator end()
        {
            return m_available.end();
        }

        Available::const_iterator cbegin() const
        {
            return m_available.cbegin();
        }

        Available::const_iterator cend() const
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
        Queue     m_queue;
    };
}
