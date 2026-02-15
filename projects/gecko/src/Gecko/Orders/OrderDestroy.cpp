#include "Gecko/Orders/OrderDestroy.hpp"

namespace Gecko
{

    OrderDestroy* OrderDestroy::create(OrderDestroy* memory, const ConfigurationPtr& configuration)
    {
        auto order = new (memory) OrderDestroy();

        order->deserialize(configuration);

        return order;
    }

    OrderDestroy::OrderDestroy() :
        base_type(order_type::Value::Destroy)
    {
    }
}
