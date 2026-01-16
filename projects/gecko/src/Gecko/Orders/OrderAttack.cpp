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

    OrderAttack* OrderAttack::create(OrderAttack* memory, const ConfigurationPtr& configuration)
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

        m_target_id = Id::Empty;
        m_target_position = Ogre::Vector3::ZERO;
    }

    ConfigurationPtr OrderAttack::serialize() const
    {
        auto configuration = base_type::serialize();

        configuration->set("target_id", m_target_id);
        configuration->set("target_position", m_target_position);

        return configuration;
    }

    void OrderAttack::deserialize(const ConfigurationPtr& configuration)
    {
        base_type::deserialize(configuration);

        m_target_id = configuration->get_int("m_target_id", Id::Empty.get());
        m_target_position = configuration->get_vector3("m_target_position", Ogre::Vector3::ZERO);
    }
}
