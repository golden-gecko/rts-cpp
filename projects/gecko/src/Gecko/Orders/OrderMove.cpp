#include "Gecko/Orders/OrderMove.hpp"

#include "Gecko/Configuration.hpp"
#include "Gecko/Components/Indicator.hpp"
#include "Gecko/Managers/ObjectManager.hpp"

namespace Gecko
{
    OrderMove* OrderMove::create()
    {
        return new OrderMove();
    }

    OrderMove* OrderMove::create(OrderMove* memory)
    {
        return new (memory) OrderMove();
    }

    OrderMove* OrderMove::create(OrderMove* memory, const ConfigurationPtr& configuration)
    {
        auto order = new (memory) OrderMove();

        order->deserialize(configuration);

        return order;
    }

    OrderMove::OrderMove() :
        base_type(order_type::Value::Move)
    {
    }

    void OrderMove::init()
    {
        base_type::init();

        set_target_position(Ogre::Vector3::ZERO);
        set_path(Navigation::Path());
    }

    ConfigurationPtr OrderMove::serialize() const
    {
        auto configuration = base_type::serialize();

        configuration->set("target_position", target_position);
        configuration->set("path", path);

        return configuration;
    }

    void OrderMove::deserialize(const ConfigurationPtr& configuration)
    {
        base_type::deserialize(configuration);

        target_position = configuration->get_vector3("target_position", Ogre::Vector3::ZERO);
        path = configuration->get_path("path", Navigation::Path());
    }

    std::vector<std::shared_ptr<Indicator>> OrderMove::generate_indicators(const std::string& material_name) const
    {
        std::shared_ptr<Indicator> indicator = std::make_shared<Cone>();

        indicator->set_material_name(material_name);
        indicator->set_position(get_target_position());

        return { indicator };
    }
}
