#include "Gecko/Orders/OrderUnload.hpp"

#include "Gecko/Configuration.hpp"

namespace Gecko
{
    OrderUnload* OrderUnload::create()
    {
        return new OrderUnload();
    }

    OrderUnload* OrderUnload::create(OrderUnload* memory)
    {
        return new (memory) OrderUnload();
    }

    OrderUnload* OrderUnload::create(OrderUnload* memory, const ConfigurationPtr& configuration)
    {
        auto order = new (memory) OrderUnload();

        order->deserialize(configuration);

        return order;
    }

    OrderUnload::OrderUnload() :
        base_type(order_type::Value::Unload)
    {
    }

    void OrderUnload::init()
    {
        base_type::init();

        set_target_id(Id::Empty);
        set_resource_name("");
        set_resource_value(0.0f);
    }

    ConfigurationPtr OrderUnload::serialize() const
    {
        auto configuration = base_type::serialize();

        configuration->set("target_id", target_id);
        configuration->set("resource_name", resource_name);
        configuration->set("resource_value", resource_value);

        return configuration;
    }

    void OrderUnload::deserialize(const ConfigurationPtr& configuration)
    {
        base_type::deserialize(configuration);

        target_id = configuration->get_int("target_id", Id::Empty.get());
        resource_name = configuration->get_string("resource_name", "");
        resource_value = configuration->get_float("resource_value", 0.0f);
    }
}
