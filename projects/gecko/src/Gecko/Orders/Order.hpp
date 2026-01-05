#pragma once

#include "Gecko/Item.hpp"
#include "Gecko/Settings.hpp"

namespace Gecko
{
    namespace order_type
    {
        enum class Value :
            std::int8_t
        {
            None,

            Attack,
            Create,
            Destroy,
            Follow,
            Guard,
            Load,
            Move,
            Patrol,
            Rally,
            Stop,
            Unload,
            Wait
        };

        std::string to_string(Value value);

        Value from_string(const std::string& value);

        inline std::ostream& operator<<(std::ostream& out, Value value)
        {
            return out << to_string(value);
        }
    }

    enum class OrderStatus :
        std::uint8_t
    {
        complete,
        complete_and_retry,
        fail,
        in_progress,
        retry
    };

    class Order :
        public Item
    {
    private:
        using base_type = Item;

    public:
        explicit Order() = default;
        explicit Order(order_type::Value type);
        explicit Order(const Order& other);

        void init() override;

        std::shared_ptr<Configuration> serialize() const override;
        void deserialize(const std::shared_ptr<Configuration>& configuration) override;

        void update(float time) override;

    public:
        bool has_reached_max_attempts_to_complete() const
        {
            return attempts_to_complete >= Settings::Game::OrderMaxAttemptsToComplete;
        }

        void increase_attempts_to_complete()
        {
            ++attempts_to_complete;
        }

    public:
        auto get_attempts_to_complete() const
        {
            return attempts_to_complete;
        }

        auto get_receiver_id() const
        {
            return receiver_id;
        }

        auto get_sender_id() const
        {
            return sender_id;
        }

        auto get_type() const
        {
            return type;
        }

    public:
        void set_receiver_id(Id _receiver_id)
        {
            receiver_id = _receiver_id;
        }

        void set_sender_id(Id _sender_id)
        {
            sender_id = _sender_id;
        }

    private:
        order_type::Value type = order_type::Value::None;

        Id sender_id;
        Id receiver_id;

        std::uint16_t attempts_to_complete = 0;
    };
}
