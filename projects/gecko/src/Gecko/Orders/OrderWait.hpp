#pragma once

#include "Gecko/Orders/Order.hpp"
#include "Gecko/Timer.hpp"

namespace Gecko
{
    class OrderWait :
        public Order
    {
    private:
        using base_type = Order;

    public:
        static OrderWait* create();
        static OrderWait* create(OrderWait* memory);
        static OrderWait* create(OrderWait* memory, const ConfigurationPtr& configuration);

    public:
        explicit OrderWait();
        explicit OrderWait(const OrderWait& other) = default;

        void init() override;

        ConfigurationPtr serialize() const override;
        void deserialize(const ConfigurationPtr& configuration) override;

    public:
        auto& get_wait_timer()
        {
            return m_wait_timer;
        }

        const auto& get_wait_timer() const
        {
            return m_wait_timer;
        }

    private:
        Timer m_wait_timer;
    };
}
