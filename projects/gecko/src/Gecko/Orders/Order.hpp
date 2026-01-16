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

        ConfigurationPtr serialize() const override;
        void deserialize(const ConfigurationPtr& configuration) override;

        void update(float time) override;

    public:
        bool has_reached_max_attempts_to_complete() const
        {
            return m_attempts_to_complete >= Settings::Game::OrderMaxAttemptsToComplete;
        }

        void increase_attempts_to_complete()
        {
            ++m_attempts_to_complete;
        }

    public:
        auto get_attempts_to_complete() const
        {
            return m_attempts_to_complete;
        }

        auto get_receiver_id() const
        {
            return m_receiver_id;
        }

        auto get_sender_id() const
        {
            return m_sender_id;
        }

        auto get_type() const
        {
            return m_type;
        }

    public:
        void set_receiver_id(Id receiver_id)
        {
            m_receiver_id = receiver_id;
        }

        void set_sender_id(Id sender_id)
        {
            m_sender_id = sender_id;
        }

    private:
        order_type::Value m_type = order_type::Value::None;

        Id m_sender_id;
        Id m_receiver_id;

        std::uint16_t m_attempts_to_complete = 0;
    };
}
