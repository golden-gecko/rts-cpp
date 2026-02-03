#include "Gecko/Objects/Missile.hpp"

#include "Gecko/Configuration.hpp"
#include "Gecko/Containers/Orders.hpp"
#include "Gecko/Layers/Layer.hpp"
#include "Gecko/Managers/OrderManager.hpp"
#include "Gecko/Maps/Map.hpp"

namespace Gecko
{
    Missile* Missile::create(Missile* memory, const ConfigurationPtr& configuration)
    {
        auto map = new (memory) Missile();

        map->deserialize(configuration);

        return map;
    }

    Missile::Missile(const Missile& other) :
        base_type(other)
    {
        m_damage = other.m_damage;
        m_speed = other.m_speed;
    }

    ConfigurationPtr Missile::serialize() const
    {
        auto configuration = base_type::serialize();

        configuration->set("damage", m_damage);
        configuration->set("speed", m_speed);

        return configuration;
    }

    void Missile::deserialize(const ConfigurationPtr& configuration)
    {
        base_type::deserialize(configuration);

        m_damage = configuration->get_float("damage", 0.0f);
        m_speed = configuration->get_float("speed", 0.0f);
    }

    void Missile::update(float time)
    {
        base_type::update(time);

        set_position(get_position() + get_direction() * m_speed * time, false);
    }

    void Missile::set_position(const Ogre::Vector3& position, bool validate)
    {
        auto map = get_owner();

        if (map)
        {
            auto orders = get_orders();
            auto& order_manager = OrderManager::getSingleton();

            if (map->is_position_valid(position) == false)
            {
                auto order = order_manager.order_destroy(get_id(), get_id());

                // TODO: Throw exception;

                orders->add_last(order->get_id());
            }
            else
            {
                auto layer_position = map->get_layer("Terrain")->get_position(position); // TODO: Hardcoded.

                if (position.y <= layer_position.y)
                {
                    auto order = order_manager.order_destroy(get_id(), get_id());

                    // TODO: Throw exception;

                    orders->add_last(order->get_id());
                }
                else
                {
                    base_type::set_position(position, validate);
                }
            }
        }
        else
        {
            base_type::set_position(position, validate);
        }
    }
}
