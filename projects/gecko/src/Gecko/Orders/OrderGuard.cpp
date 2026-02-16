#include "Gecko/Orders/OrderGuard.hpp"

#include "Gecko/Configuration.hpp"

namespace Gecko
{
    void OrderGuard::init()
    {
        base_type::init();

        set_target_id(Id::Empty);
        set_target_position(Ogre::Vector3::ZERO);
        set_path(Navigation::Path());
    }

    ConfigurationPtr OrderGuard::serialize() const
    {
        auto configuration = base_type::serialize();

        configuration->set("target_id", target_id);
        configuration->set("target_position", target_position);
        configuration->set("path", path);

        return configuration;
    }

    void OrderGuard::deserialize(const ConfigurationPtr& configuration)
    {
        base_type::deserialize(configuration);

        target_id = configuration->get_int("target_id", Id::Empty.get());
        target_position = configuration->get_vector3("target_position", Ogre::Vector3::ZERO);
        path = configuration->get_path("path", Navigation::Path());
    }

    OrderGuard* OrderGuard::create(OrderGuard* memory, const ConfigurationPtr& configuration)
    {
        auto order = new (memory) OrderGuard();

        order->deserialize(configuration);

        return order;
    }

    OrderGuard::OrderGuard() :
        base_type(order_type::Value::Guard)
    {
    }
}
