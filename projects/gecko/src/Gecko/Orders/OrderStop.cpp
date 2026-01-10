#include "Gecko/Orders/OrderStop.hpp"

namespace Gecko
{
    OrderStop* OrderStop::create()
    {
        return new OrderStop();
    }

    OrderStop* OrderStop::create(OrderStop* memory)
    {
        return new (memory) OrderStop();
    }

    OrderStop* OrderStop::create(OrderStop* memory, const ConfigurationPtr& configuration)
    {
        auto order = new (memory) OrderStop();

        order->deserialize(configuration);

        return order;
    }

    OrderStop::OrderStop() :
        base_type(order_type::Value::Stop)
    {
    }
}
