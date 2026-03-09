#pragma once

#include "Gecko/Orders/Order.hpp"

namespace Gecko
{
    class OrderDestroy :
        public Order
    {
    private:
        using base_type = Order;

    public:
        static OrderDestroy* create(OrderDestroy* memory, const ConfigurationPtr& configuration);

    public:
        explicit OrderDestroy();
        OrderDestroy(const OrderDestroy& other) = default;
    };
}
