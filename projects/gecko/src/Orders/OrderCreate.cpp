#include "Orders/OrderCreate.hpp"

#include "Configuration.hpp"

namespace Gecko
{
    OrderCreate* OrderCreate::create()
    {
        return new OrderCreate();
    }

    OrderCreate* OrderCreate::create(OrderCreate* memory)
    {
        return new (memory) OrderCreate();
    }

    OrderCreate* OrderCreate::create(OrderCreate* memory, const std::shared_ptr<Configuration>& configuration)
    {
        auto order = new (memory) OrderCreate();

        order->deserialize(configuration);

        return order;
    }

    OrderCreate::OrderCreate() :
        base_type(order_type::Value::Create)
    {
    }

    void OrderCreate::init()
    {
        base_type::init();

        set_player_id(Id::Empty);
        set_configuration_name("");
        set_position(Ogre::Vector3::ZERO);
    }

    std::shared_ptr<Configuration> OrderCreate::serialize() const
    {
        auto configuration = base_type::serialize();

        configuration->set("player_id", player_id);
        configuration->set("configuration_name", configuration_name);
        configuration->set("position", position);

        return configuration;
    }

    void OrderCreate::deserialize(const std::shared_ptr<Configuration>& configuration)
    {
        base_type::deserialize(configuration);

        player_id = configuration->get_int("player_id", Id::Empty.get());
        configuration_name = configuration->get_string("configuration_name", "");
        position = configuration->get_vector3("position", Ogre::Vector3::ZERO);
    }
}
