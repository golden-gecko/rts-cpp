#pragma once

#include "Orders/Order.hpp"

namespace Gecko
{
    class OrderStop : public Order
    {
    private:
        using base_type = Order;

    public:
        static OrderStop* create();
        static OrderStop* create(OrderStop* memory);
        static OrderStop* create(OrderStop* memory, const std::shared_ptr<Configuration>& configuration);

    public:
        explicit OrderStop();
        explicit OrderStop(const OrderStop& other) = default;
    };
}
