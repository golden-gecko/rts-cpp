#include "Gecko/Orders/OrderWait.hpp"

#include "Gecko/Configuration.hpp"

namespace Gecko
{
    void OrderWait::init()
    {
        base_type::init();

        m_wait_timer.reset();
    }

    ConfigurationPtr OrderWait::serialize() const
    {
        auto configuration = base_type::serialize();

        configuration->set("wait_timer", m_wait_timer.serialize());

        return configuration;
    }

    void OrderWait::deserialize(const ConfigurationPtr& configuration)
    {
        base_type::deserialize(configuration);

        if (configuration->has_member("wait_timer"))
        {
            m_wait_timer.deserialize(configuration->get_child("wait_timer"));
        }
    }

    OrderWait* OrderWait::create(OrderWait* memory, const ConfigurationPtr& configuration)
    {
        auto order = new (memory) OrderWait();

        order->deserialize(configuration);

        return order;
    }

    OrderWait::OrderWait() :
        base_type(order_type::Value::Wait)
    {
    }

    std::string OrderWait::get_target_as_string() const
    {
        std::stringstream stream; // TODO: Move to utils.

        stream << std::fixed << std::setprecision(1);
        stream << m_wait_timer.get_current() << "/" << m_wait_timer.get_max();

        return stream.str();
    }
}
