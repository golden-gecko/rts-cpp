#pragma once

#include "Gecko/Orders/Order.hpp"

namespace Gecko
{
    class OrderStop :
        public Order
    {
    private:
        using base_type = Order;

    public:
        static OrderStop* create(OrderStop* memory, const ConfigurationPtr& configuration);

    public:
        explicit OrderStop();
        OrderStop(const OrderStop& other) = default;
    };
}
