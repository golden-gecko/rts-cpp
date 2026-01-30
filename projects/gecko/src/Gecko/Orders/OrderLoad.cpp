#include "Gecko/Orders/OrderLoad.hpp"

#include "Gecko/Configuration.hpp"
#include "Gecko/Managers/ObjectManager.hpp"
#include "Gecko/Objects/Object.hpp"
#include "Gecko/UI/Indicators/Point.hpp"

namespace Gecko
{
    OrderLoad* OrderLoad::create()
    {
        return new OrderLoad();
    }

    OrderLoad* OrderLoad::create(OrderLoad* memory)
    {
        return new (memory) OrderLoad();
    }

    OrderLoad* OrderLoad::create(OrderLoad* memory, const ConfigurationPtr& configuration)
    {
        auto order = new (memory) OrderLoad();

        order->deserialize(configuration);

        return order;
    }

    OrderLoad::OrderLoad() :
        base_type(order_type::Value::Load)
    {
    }

    void OrderLoad::init()
    {
        base_type::init();

        set_target_id(Id::Empty);
        set_resource_name("");
        set_resource_value(0.0f);
    }

    ConfigurationPtr OrderLoad::serialize() const
    {
        auto configuration = base_type::serialize();

        configuration->set("target_id", target_id);
        configuration->set("resource_name", resource_name);
        configuration->set("resource_value", resource_value);

        return configuration;
    }

    void OrderLoad::deserialize(const ConfigurationPtr& configuration)
    {
        base_type::deserialize(configuration);

        target_id = configuration->get_int("target_id", Id::Empty.get());
        resource_name = configuration->get_string("resource_name", "");
        resource_value = configuration->get_float("resource_value", 0.0f);
    }

    std::vector<std::shared_ptr<Indicator>> OrderLoad::generate_indicators(int order_number) const
    {
        std::shared_ptr<Indicator> indicator = std::make_shared<Point>();

        if (ObjectPtr target = ObjectManager::getSingleton().get(get_target_id()))
        {
            indicator->set_material_name(std::format("cone_{}", order_number));
            indicator->set_position(target->get_position() + target->get_size() * Ogre::Vector3::UNIT_Y);
        }

        return { indicator };
    }
}
