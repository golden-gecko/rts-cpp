#include "Orders/OrderLoad.hpp"

#include "Configuration.hpp"

namespace Gecko
{
    OrderLoad* OrderLoad::create()
    {
        return new OrderLoad();
    }

    OrderLoad* OrderLoad::create(OrderLoad* memory)
    {
        return new (memory) OrderLoad();
    }

    OrderLoad* OrderLoad::create(OrderLoad* memory, const std::shared_ptr<Configuration>& configuration)
    {
        auto order = new (memory) OrderLoad();

        order->deserialize(configuration);

        return order;
    }

    OrderLoad::OrderLoad() :
        base_type(order_type::Value::Load)
    {
    }

    void OrderLoad::init()
    {
        base_type::init();

        set_target_id(Id::Empty);
        set_resource_name("");
        set_resource_value(0.0f);
    }

    std::shared_ptr<Configuration> OrderLoad::serialize() const
    {
        auto configuration = base_type::serialize();

        configuration->set("target_id", target_id);
        configuration->set("resource_name", resource_name);
        configuration->set("resource_value", resource_value);

        return configuration;
    }

    void OrderLoad::deserialize(const std::shared_ptr<Configuration>& configuration)
    {
        base_type::deserialize(configuration);

        target_id = configuration->get_int("target_id", Id::Empty.get());
        resource_name = configuration->get_string("resource_name", "");
        resource_value = configuration->get_float("resource_value", 0.0f);
    }
}
