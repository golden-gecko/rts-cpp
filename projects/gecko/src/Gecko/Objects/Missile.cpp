#include "Gecko/Objects/Missile.hpp"

#include "Gecko/Configuration.hpp"
#include "Gecko/Containers/Orders.hpp"
#include "Gecko/Layers/Layer.hpp"
#include "Gecko/Log.hpp"
#include "Gecko/Managers/OrderManager.hpp"
#include "Gecko/Maps/Map.hpp"

namespace Gecko
{
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

    Missile* Missile::create(Missile* memory, const ConfigurationPtr& configuration, const ScenePtr& scene)
    {
        auto map = new (memory) Missile(scene);

        map->deserialize(configuration);

        return map;
    }

    Missile::Missile(const ScenePtr& scene) :
        base_type(scene)
    {
    }

    void Missile::set_position(const Ogre::Vector3& position, bool validate)
    {
        if (MapPtr map = get_owner())
        {
            if (map->is_position_valid(position) == false)
            {
                if (OrderPtr order = OrderManager::getSingleton().order_destroy(get_id(), get_id()))
                {
                    get_orders()->add_last(order->get_id());
                }
                else
                {
                    L_WARNING << "Failed to create 'Destroy' order.";
                }
            }
            else
            {
                if (LayerPtr layer = map->get_layer(Settings::Layer::Terrain))
                {
                    Ogre::Vector3 layer_position = layer->get_position(position);

                    if (position.y <= layer_position.y)
                    {
                        if (OrderPtr order = OrderManager::getSingleton().order_destroy(get_id(), get_id()))
                        {
                            get_orders()->add_last(order->get_id());
                        }
                        else
                        {
                            L_WARNING << "Failed to create 'Destroy' order.";
                        }
                    }
                    else
                    {
                        base_type::set_position(position, validate);
                    }
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
