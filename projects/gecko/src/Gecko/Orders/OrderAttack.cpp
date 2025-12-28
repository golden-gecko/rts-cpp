#include "Gecko/Orders/OrderAttack.hpp"

#include "Gecko/Configuration.hpp"
#include "Gecko/Exception.hpp"

namespace Gecko
{
    OrderAttack* OrderAttack::create()
    {
        return new OrderAttack();
    }

    OrderAttack* OrderAttack::create(OrderAttack* memory)
    {
        return new (memory) OrderAttack();
    }

    OrderAttack* OrderAttack::create(OrderAttack* memory, const std::shared_ptr<Configuration>& configuration)
    {
        auto order = new (memory) OrderAttack();

        order->deserialize(configuration);

        return order;
    }

    OrderAttack::OrderAttack() :
        base_type(order_type::Value::Attack)
    {
    }

    void OrderAttack::init()
    {
        base_type::init();

        target_id = Id::Empty;
        target_position = Ogre::Vector3::ZERO;
    }

    std::shared_ptr<Configuration> OrderAttack::serialize() const
    {
        auto configuration = base_type::serialize();

        configuration->set("target_id", target_id);
        configuration->set("target_position", target_position);

        return configuration;
    }

    void OrderAttack::deserialize(const std::shared_ptr<Configuration>& configuration)
    {
        base_type::deserialize(configuration);

        target_id = configuration->get_int("target_id", Id::Empty.get());
        target_position = configuration->get_vector3("target_position", Ogre::Vector3::ZERO);
    }
}
