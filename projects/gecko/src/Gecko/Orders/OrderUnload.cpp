#include "Gecko/Orders/OrderUnload.hpp"

#include "Gecko/Configuration.hpp"
#include "Gecko/Components/Indicator.hpp"
#include "Gecko/Managers/ObjectManager.hpp"
#include "Gecko/Objects/Object.hpp"

namespace Gecko
{
    OrderUnload* OrderUnload::create()
    {
        return new OrderUnload();
    }

    OrderUnload* OrderUnload::create(OrderUnload* memory)
    {
        return new (memory) OrderUnload();
    }

    OrderUnload* OrderUnload::create(OrderUnload* memory, const ConfigurationPtr& configuration)
    {
        auto order = new (memory) OrderUnload();

        order->deserialize(configuration);

        return order;
    }

    OrderUnload::OrderUnload() :
        base_type(order_type::Value::Unload)
    {
    }

    void OrderUnload::init()
    {
        base_type::init();

        set_target_id(Id::Empty);
        set_resource_name("");
        set_resource_value(0.0f);
    }

    ConfigurationPtr OrderUnload::serialize() const
    {
        auto configuration = base_type::serialize();

        configuration->set("target_id", m_target_id);
        configuration->set("resource_name", m_resource_name);
        configuration->set("resource_value", m_resource_value);

        return configuration;
    }

    void OrderUnload::deserialize(const ConfigurationPtr& configuration)
    {
        base_type::deserialize(configuration);

        m_target_id = configuration->get_int("target_id", Id::Empty.get());
        m_resource_name = configuration->get_string("resource_name", "");
        m_resource_value = configuration->get_float("resource_value", 0.0f);
    }

    std::vector<std::shared_ptr<Indicator>> OrderUnload::generate_indicators(const std::string& material_name) const
    {
        std::shared_ptr<Indicator> indicator = std::make_shared<Cone>();

        if (ObjectPtr target = ObjectManager::getSingleton().get(get_target_id()))
        {
            indicator->set_material_name(material_name);
            indicator->set_position(target->get_position());
        }

        return { indicator };
    }
}
