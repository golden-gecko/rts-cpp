#pragma once

#include "Gecko/Interfaces/Serializable.hpp"
#include "Gecko/Orders/Order.hpp"

namespace Gecko
{
    class Orders : public Serializable
    {
    public:
        typedef std::set<order_type::Value> Available;
        typedef std::deque<Id> Queue;

    public:
        ~Orders();

        std::shared_ptr<Configuration> serialize() const override;
        void deserialize(const std::shared_ptr<Configuration>& configuration) override;

    public:
        void add(const order_type::Value& item)
        {
            available.emplace(item);
        }

        bool add_first(Id order_id);
        bool add_last(Id order_id);

        void clear()
        {
            queue.clear();
        }

        auto empty() const
        {
            return queue.empty();
        }

        const auto& front() const
        {
            return queue.front();
        }

        bool is_available(order_type::Value item) const
        {
            return std::ranges::find(available, item) != available.end();
        }

        void move_first_to_end();

        void remove(Id order_id);
        void remove_current_order();
        void remove_all_orders();

        auto size() const
        {
            return queue.size();
        }

    public:
        // TODO: Remove.
        auto& get_available()
        {
            return available;
        }

        const auto& get_available() const
        {
            return available;
        }

        // TODO: Remove.
        auto& get_queue()
        {
            return queue;
        }

        const auto& get_queue() const
        {
            return queue;
        }

    public:
        // TODO: Iterate over available or queue?
        auto begin() const
        {
            return available.begin();
        }

        auto end() const
        {
            return available.end();
        }

        auto cbegin() const
        {
            return available.cbegin();
        }

        auto cend() const
        {
            return available.cend();
        }

    public:
        bool operator==(const Orders& other) const
        {
            return available == other.available && queue == other.queue;
        }

    private:
        Available available;
        Queue queue;
    };
}
