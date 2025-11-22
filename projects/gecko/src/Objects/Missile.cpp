#include "Objects/Missile.hpp"

#include "Configuration.hpp"
#include "Containers/Orders.hpp"
#include "Layers/Layer.hpp"
#include "Managers/OrderManager.hpp"
#include "Maps/Map.hpp"

namespace Gecko
{
    Missile* Missile::create(Missile* memory, const std::shared_ptr<Configuration>& configuration)
    {
        auto map = new (memory) Missile();

        map->deserialize(configuration);

        return map;
    }

    Missile::Missile(const Missile& other) :
        base_type(other)
    {
        damage = other.damage;
        speed = other.speed;
    }

    std::shared_ptr<Configuration> Missile::serialize() const
    {
        auto configuration = base_type::serialize();

        configuration->set("damage", damage);
        configuration->set("speed", speed);

        return configuration;
    }

    void Missile::deserialize(const std::shared_ptr<Configuration>& configuration)
    {
        base_type::deserialize(configuration);

        damage = configuration->get_float("damage", 0.0f);
        speed = configuration->get_float("speed", 0.0f);
    }

    void Missile::update(float time)
    {
        base_type::update(time);

        set_position(get_position() + get_direction() * speed * time, false);
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
                // TODO: Hardcoded layer name.
                auto layer_position = map->get_layer("Terrain")->get_position(position);

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
