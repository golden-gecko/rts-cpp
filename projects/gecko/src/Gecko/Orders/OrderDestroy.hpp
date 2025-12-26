#pragma once

#include "Gecko/Orders/Order.hpp"

namespace Gecko
{
    class OrderDestroy : public Order
    {
    private:
        using base_type = Order;

    public:
        static OrderDestroy* create();
        static OrderDestroy* create(OrderDestroy* memory);
        static OrderDestroy* create(OrderDestroy* memory, const std::shared_ptr<Configuration>& configuration);

    public:
        explicit OrderDestroy();
        explicit OrderDestroy(const OrderDestroy& other) = default;
    };
}
