#include "Gecko/Orders/OrderFollow.hpp"

#include "Gecko/Configuration.hpp"

namespace Gecko
{
    OrderFollow* OrderFollow::create()
    {
        return new OrderFollow();
    }

    OrderFollow* OrderFollow::create(OrderFollow* memory)
    {
        return new (memory) OrderFollow();
    }

    OrderFollow* OrderFollow::create(OrderFollow* memory, const ConfigurationPtr& configuration)
    {
        auto order = new (memory) OrderFollow();

        order->deserialize(configuration);

        return order;
    }

    OrderFollow::OrderFollow() :
        base_type(order_type::Value::Follow)
    {
    }

    void OrderFollow::init()
    {
        base_type::init();

        set_path(Path());
        set_target_id(Id::Empty);
    }

    ConfigurationPtr OrderFollow::serialize() const
    {
        auto configuration = base_type::serialize();

        configuration->set("target_id", target_id);
        configuration->set("path", path);

        return configuration;
    }

    void OrderFollow::deserialize(const ConfigurationPtr& configuration)
    {
        base_type::deserialize(configuration);

        target_id = configuration->get_int("target_id", Id::Empty.get());
        path = configuration->get_path("path", Path());
    }
}
