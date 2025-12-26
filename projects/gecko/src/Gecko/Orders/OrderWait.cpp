#include "Gecko/Orders/OrderWait.hpp"

#include "Gecko/Configuration.hpp"

namespace Gecko
{
    OrderWait* OrderWait::create()
    {
        return new OrderWait();
    }

    OrderWait* OrderWait::create(OrderWait* memory)
    {
        return new (memory) OrderWait();
    }

    OrderWait* OrderWait::create(OrderWait* memory, const std::shared_ptr<Configuration>& configuration)
    {
        auto order = new (memory) OrderWait();

        order->deserialize(configuration);

        return order;
    }

    OrderWait::OrderWait() :
        base_type(order_type::Value::Wait)
    {
    }

    void OrderWait::init()
    {
        base_type::init();

        wait_timer.reset();
    }

    std::shared_ptr<Configuration> OrderWait::serialize() const
    {
        auto configuration = base_type::serialize();

        configuration->set("wait_timer", wait_timer.serialize());

        return configuration;
    }

    void OrderWait::deserialize(const std::shared_ptr<Configuration>& configuration)
    {
        base_type::deserialize(configuration);

        if (configuration->has_member("wait_timer"))
        {
            wait_timer.deserialize(configuration->get_child("wait_timer"));
        }
    }
}
