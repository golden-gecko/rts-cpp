#include "Objects/Unit.hpp"

#include "Cameras/Camera.hpp"
#include "ConfigurationManager.hpp"
#include "Convert.hpp"
#include "Exception.hpp"
#include "Game.hpp"
#include "Maps/Map.hpp"
#include "ObjectManager.hpp"
#include "Orders/OrderAttack.hpp"
#include "Orders/OrderFollow.hpp"
#include "Orders/OrderGuard.hpp"
#include "Path.hpp"
#include "PlayerManager.hpp"
#include "Statistics.hpp"

namespace Gecko
{
    Unit* Unit::create(Unit* memory, std::shared_ptr<Configuration> configuration)
    {
        auto map = new (memory) Unit();

        map->deserialize(configuration);

        return map;
    }

    Unit::Unit()
    {
    }

    Unit::Unit(const Unit& other) :
        base_type(other)
    {
    }

    std::shared_ptr<Configuration> Unit::serialize() const
    {
        auto configuration = base_type::serialize();

        return configuration;
    }

    void Unit::deserialize(std::shared_ptr<Configuration> _configuration)
    {
        base_type::deserialize(_configuration);

        /*
        speed = configuration.get_float("speed", 0.0f);
        reload_timer.set_maximal(configuration.get_float("reload_rate", 1.0f));
        range = configuration.get_float("range", 1.0f);
        missile_configuration = configuration.get_string("missile", "");
        */
    }

    void Unit::update(float time)
    {
        base_type::update(time);

        /*
        btTransform trans;

        auto motion_state = rigid_body->getMotionState();
        motion_state->getWorldTransform(trans);

        // auto origin = trans.getOrigin();
        // origin.setX(origin.getX() + 2.0f * time);
        // trans.setOrigin(origin);
        // rigid_body->getMotionState()->setWorldTransform(trans);

        rigid_body->activate(true);
        rigid_body->applyForce(btVector3(2.0f, 0.0f, 0.0f), btVector3(0.0f, 0.0f, 0.0f));

        scene_node->setPosition(
            trans.getOrigin().getX(),
            trans.getOrigin().getY(),
            trans.getOrigin().getZ()
        );

        auto rotation = trans.getRotation();

        scene_node->setOrientation(Ogre::Quaternion(
            rotation.getW(),
            rotation.getX(), 
            rotation.getY(),
            rotation.getZ()
        ));

        // TODO: Refactor.
        if (get_initialized() && is_enabled())
        {
            reload_timer.update(time);

            if (orders.size())
            {
                auto order = orders.front();

                if (order)
                {
                    switch (order->get_type())
                    {
                        case order_type::Value::Attack:
                        {
                            auto attack_order = dynamic_cast<OrderAttack*>(order);

                            if (attack_order)
                            {
                                if (reload_timer)
                                {
                                    auto target_id = attack_order->get_target_id();

                                    if (target_id)
                                    {
                                        auto object = ObjectManager::getSingleton().get(target_id);

                                        if (object)
                                        {
                                            attack_position(object->get_position());
                                        }
                                        else
                                        {
                                            remove_current_order();
                                        }
                                    }
                                    else
                                    {
                                        attack_position(attack_order->get_target_position());
                                    }
                                }
                            }

                            break;
                        }

                        case order_type::Value::Destroy:
                            on_destroy();
                            break;

                        case order_type::Value::Follow:
                        {
                            auto follow_order = dynamic_cast<OrderFollow*>(order);

                            if (follow_order)
                            {
                                auto object = ObjectManager::getSingleton().get(follow_order->get_target_id());

                                if (object)
                                {
                                    move_towards_position(object->get_position(), time, false);
                                }
                                else
                                {
                                    remove_current_order();
                                }
                            }

                            break;
                        }

                        case order_type::Value::Guard:
                        {
                            auto guard_order = dynamic_cast<OrderGuard*>(order);

                            if (guard_order)
                            {
                                auto target_id = guard_order->get_target_id();

                                if (target_id)
                                {
                                    auto object = ObjectManager::getSingleton().get(guard_order->get_target_id());

                                    if (object)
                                    {
                                        move_towards_position(object->get_position(), time, false);
                                    }
                                    else
                                    {
                                        remove_current_order();
                                    }
                                }
                                else
                                {
                                    move_towards_position(guard_order->get_target_position(), time);
                                }
                            }

                            break;
                        }

                        case order_type::Value::Move:
                        {
                            auto order_move = dynamic_cast<OrderMove*>(order);

                            if (order_move)
                            {
                                move_towards_position(order_move->get_target_position(), time);
                            }

                            break;
                        }

                        default:
                            remove_current_order();
                    }
                }
            }
            else
            {
                ai_timer.update(time);

                if (ai_timer)
                {
                    ai_timer.reset();

                    if (reload_timer)
                    {
                        // Find and attack enemy in range.
                        auto objects = ObjectManager::getSingleton().get(get_position(), get_range());

                        for (const auto& object_id : objects)
                        {
                            auto object = ObjectManager::getSingleton().get(object_id.second);

                            if (object)
                            {
                                auto player = PlayerManager::getSingleton().get(get_player_id());

                                if (player)
                                {
                                    if (player->is(object->get_player_id(), Diplomacy::State::Enemy))
                                    {
                                        attack_position(object->get_position());

                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        */
    }

    /*
    ObjectAttributes Unit::to_string() const
    {
        auto attributes = base_type::to_string();

        attributes["Attributes"]["Attributes"] = speed;
        attributes["Attributes"]["Reload"] = reload_timer.to_string();
        attributes["Attributes"]["Range"] = get_range();
        attributes["Attributes"]["Distance traveled"] = distance_traveled;
        attributes["Attributes"]["Pollution produced"] = pollution_produced;
        attributes["Attributes"]["Missiles fired"] = missiles_fired;

        return attributes;
    }
    */

#if 0
    std::map<std::string, float> Unit::get_bars() const
    {
        auto bars = Object::get_bars();

        if (reload_timer.get_maximal() == 0.0f)
        {
            bars["reload"] = 0.0f;
        }
        else
        {
            bars["reload"] = std::roundf(reload_timer.get_current() / reload_timer.get_maximal() * 100.f);
        }

        if (get_max_fuel() == 0.0f)
        {
            bars["fuel"] = 0.0f;
        }
        else
        {
            bars["fuel"] = std::roundf(get_fuel() / get_max_fuel() * 100.f);
        }

        return bars;
    }

    void Unit::attack_position(const Ogre::Vector3& position)
    {
        auto missile = ObjectManager::getSingleton().create(missile_configuration);

        /* TODO: Fix.
        if (missile)
        {
            auto p1 = Game::getSingleton().get_active_map()->get_terrain().get_position(position.x, position.z) + 1.0f;
            auto p2 = get_position();
            auto p3 = p1 - p2;
            auto p4 = p3.normalisedCopy();

            set_direction(p4);

            missile->set_direction(p4);
            missile->set_player_id(get_player_id());
            missile->set_position(get_position());

            ++missiles_fired;
            Statistics::getSingleton().add("Missiles fired", 1.0f);
        }
        */

        reload_timer.reset();
    }

    void Unit::move_towards_position(const Ogre::Vector3& position, float time, bool remove_order)
    {
        // If there is not path, find one.
        auto current = Ogre::Vector3(get_position().x, 0.0f, get_position().z);
        auto target = Ogre::Vector3(position.x, 0.0f, position.z);

        if (path.size() == 0)
        {
            /*
            TODO: Fix.
            path = Game::getSingleton().get_active_map()->get_terrain().get_path(current, target);

            // If target cannot be reached, remove order.
            if (path.size() == 0 && remove_order)
            {
                get_orders().remove_current_order();

                return;
            }
            */
        }

        // If first point in path was reached, remove that point.
        // Otherwise, move towards first point.
        target = Ogre::Vector3(path.front().x, 0.0f, path.front().z);

        auto move_vector = (target - current).normalisedCopy() * speed * time;
        auto move_distance = move_vector.length();

        if (current.distance(target) > move_distance)
        {
            set_direction(move_vector.normalisedCopy());
            set_position(get_position() + move_vector);

            auto distance = move_vector.length();
            auto pollution = move_vector.length() * 0.1f;

            distance_traveled += distance;
            pollution_produced += pollution;

            Statistics::getSingleton().add("Distance traveled", distance);
            Statistics::getSingleton().add("Pollution produced", pollution);

            // TODO: Split pollution between all traveled tiles.
            // map->get_terrain().add_pollution(get_position(), pollution);
        }
        else
        {
            set_direction(move_vector.normalisedCopy());
            set_position(target);

            auto distance = current.distance(target);
            auto pollution = current.distance(target) * 0.1f;

            distance_traveled += distance;
            pollution_produced += pollution;

            Statistics::getSingleton().add("Distance traveled", distance);
            Statistics::getSingleton().add("Pollution produced", pollution);

            // TODO: Split pollution between all traveled tiles.
            // map->get_terrain().add_pollution(get_position(), pollution);

            // If reached point is a portal, teleport to next portal and both portals from the path.
            // Otherwise, remove reached point.
            auto first = path.front();
            path.pop_front();

            /*
            if (map->get_terrain().is_portal(first))
            {
                if (path.size() == 0)
                {
                    throw Exception("Path has single portal.");
                }

                auto next = path.front();
                path.pop_front();

                if (map->get_terrain().is_portal(next) == false)
                {
                    throw Exception("Path has single portal.");
                }

                // Teleport.
                set_position(next);
            }
            */

            // If last point was reached, remove order.
            if (path.size() == 0 && remove_order)
            {
                get_orders().remove_current_order();
            }
        }
    }
#endif
}
