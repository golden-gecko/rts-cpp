#include "Gecko/Orders/OrderRally.hpp"

#include "Gecko/Configuration.hpp"

namespace Gecko
{
    OrderRally* OrderRally::create()
    {
        return new OrderRally();
    }

    OrderRally* OrderRally::create(OrderRally* memory)
    {
        return new (memory) OrderRally();
    }

    OrderRally* OrderRally::create(OrderRally* memory, const std::shared_ptr<Configuration>& configuration)
    {
        auto order = new (memory) OrderRally();

        order->deserialize(configuration);

        return order;
    }

    OrderRally::OrderRally() :
        base_type(order_type::Value::Rally)
    {
    }

    void OrderRally::init()
    {
        base_type::init();

        set_target_position(Ogre::Vector3::ZERO);
    }

    std::shared_ptr<Configuration> OrderRally::serialize() const
    {
        auto configuration = base_type::serialize();

        configuration->set("target_id", target_id);
        configuration->set("target_position", target_position);

        return configuration;
    }

    void OrderRally::deserialize(const std::shared_ptr<Configuration>& configuration)
    {
        base_type::deserialize(configuration);

        target_id = configuration->get_int("target_id", Id::Empty.get());
        target_position = configuration->get_vector3("target_position", Ogre::Vector3::ZERO);
    }
}
