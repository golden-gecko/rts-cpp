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
        static OrderWait* create(OrderWait* memory, const std::shared_ptr<Configuration>& configuration);

    public:
        explicit OrderWait();
        explicit OrderWait(const OrderWait& other) = default;

        void init() override;

        std::shared_ptr<Configuration> serialize() const override;
        void deserialize(const std::shared_ptr<Configuration>& configuration) override;

    public:
        auto& get_wait_timer()
        {
            return wait_timer;
        }

        const auto& get_wait_timer() const
        {
            return wait_timer;
        }

    private:
        Timer wait_timer;
    };
}
