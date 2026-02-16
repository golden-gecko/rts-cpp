#include "Gecko/Orders/OrderRally.hpp"

#include "Gecko/Configuration.hpp"

namespace Gecko
{
    void OrderRally::init()
    {
        base_type::init();

        set_target_position(Ogre::Vector3::ZERO);
    }

    ConfigurationPtr OrderRally::serialize() const
    {
        auto configuration = base_type::serialize();

        configuration->set("target_id", m_target_id);
        configuration->set("target_position", m_target_position);

        return configuration;
    }

    void OrderRally::deserialize(const ConfigurationPtr& configuration)
    {
        base_type::deserialize(configuration);

        m_target_id = configuration->get_int("target_id", Id::Empty.get());
        m_target_position = configuration->get_vector3("target_position", Ogre::Vector3::ZERO);
    }

    OrderRally* OrderRally::create(OrderRally* memory, const ConfigurationPtr& configuration)
    {
        auto order = new (memory) OrderRally();

        order->deserialize(configuration);

        return order;
    }

    OrderRally::OrderRally() :
        base_type(order_type::Value::Rally)
    {
    }
}
