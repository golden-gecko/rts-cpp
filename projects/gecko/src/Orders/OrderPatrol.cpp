#include "Orders/OrderPatrol.hpp"

#include "Configuration.hpp"

namespace Gecko
{
    OrderPatrol* OrderPatrol::create()
    {
        return new OrderPatrol();
    }

    OrderPatrol* OrderPatrol::create(OrderPatrol* memory)
    {
        return new (memory) OrderPatrol();
    }

    OrderPatrol* OrderPatrol::create(OrderPatrol* memory, const std::shared_ptr<Configuration>& configuration)
    {
        auto order = new (memory) OrderPatrol();

        order->deserialize(configuration);

        return order;
    }

    OrderPatrol::OrderPatrol() :
        base_type(order_type::Value::Patrol)
    {
    }

    void OrderPatrol::init()
    {
        base_type::init();

        set_target_id(Id::Empty);
        set_target_position(Ogre::Vector3::ZERO);
        set_path(Path());
    }

    std::shared_ptr<Configuration> OrderPatrol::serialize() const
    {
        auto configuration = base_type::serialize();

        configuration->set("target_id", target_id);
        configuration->set("target_position", target_position);
        configuration->set("path", path);

        return configuration;
    }

    void OrderPatrol::deserialize(const std::shared_ptr<Configuration>& configuration)
    {
        base_type::deserialize(configuration);

        target_id = configuration->get_int("target_id", Id::Empty.get());
        target_position = configuration->get_vector3("target_position", Ogre::Vector3::ZERO);
        path = configuration->get_path("path", Path());
    }
}
