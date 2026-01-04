#include "Citizen.hpp"

#include <Gecko/Containers/Orders.hpp>
#include <Gecko/Containers/Resources.hpp>
#include <Gecko/Games/Game.hpp>
#include <Gecko/Log.hpp>
#include <Gecko/Managers/JobManager.hpp>
#include <Gecko/Managers/ObjectManager.hpp>
#include <Gecko/Managers/OrderManager.hpp>
#include <Gecko/Maps/Map.hpp>
#include <Gecko/Orders/OrderLoad.hpp>
#include <Gecko/Orders/OrderMove.hpp>
#include <Gecko/Orders/OrderUnload.hpp>
#include <Gecko/Search/FollowRoadsSearch.hpp>
#include <Gecko/Utils/Utils.hpp>

namespace City
{
    /*
    Citizen::Citizen(const std::string& configuration_name) :
        base_type(configuration_name)
    {
        speed = configuration->get_float("speed");

        home_time.set_maximal(configuration->get_float("home_time"));
        workplace_time.set_maximal(configuration->get_float("workplace_time"));
    }
    */

    Citizen::Citizen(const Citizen& other) :
        base_type(other)
    {
        speed = other.speed;

        home_time = other.home_time;
        workplace_time = other.workplace_time;
    }

    void Citizen::update(float time)
    {
        base_type::update(time);

        /*
        if (get_initialized() && is_enabled())
        {
            if (update_timer.update(time))
            {
                update_timer.reset();

                // TODO: Remove singleton?
                auto city_object_manager = dynamic_cast<ObjectManager*>(ObjectManager::getSingletonPtr());

                // Find home.
                if (home_id.is_valid() == false)
                {
                    // TODO: Check if home_id exists.
                    home_id = city_object_manager->get_empty_home();

                    if (home_id.is_valid())
                    {
                        auto home = Gecko::ObjectManager::getSingleton().get<CityBuilding>(home_id);

                        set_home_id(home->get_id());
                        home->assign_resident(get_id());

                        // Move to home.
                        auto home_entrance = home->get_entrance();

                        if (home_entrance)
                        {
                            get_orders()->add_last(
                                Gecko::OrderManager::getSingletonPtr()->order_move(
                                    get_id(), get_id(), home_entrance.value().position
                                )
                            );
                        }
                    }
                }

                // Find workplace.
                if (workplace_id.is_valid() == false)
                {
                    // TODO: Check if workplace_id exists.
                    workplace_id = city_object_manager->get_empty_workplace();

                    if (workplace_id.is_valid())
                    {
                        auto workplace = Gecko::ObjectManager::getSingleton().get<CityBuilding>(workplace_id);

                        set_workplace_id(workplace_id);
                        workplace->assign_worker(get_id());
                    }
                }
            }
        }
        */
    }

    /*
    Gecko::ObjectAttributes Citizen::to_string() const
    {
        auto attributes = base_type::to_string();

        attributes["Attributes"]["Distance"] = Gecko::Convert::to_string(distance);
        attributes["Attributes"]["Speed"] = Gecko::Convert::to_string(speed);
        attributes["City"]["Home ID"] = Gecko::Convert::to_string(get_home_id());
        attributes["City"]["Workplace ID"] = Gecko::Convert::to_string(get_workplace_id());

        return attributes;
    }
    */

    void Citizen::set_position(const Ogre::Vector3& position, bool validate)
    {
        base_type::set_position(position, validate);

        /*
        TODO: Fix.
        // Get previous position.
        auto previous_position = get_position();

        // Set position.
        base_type::set_position(position);

        // Get current position.
        auto current_position = get_position();

        // Update terrain layer.
        auto& terrain = Gecko::Game::getSingleton().get_active_map()->get_terrain();

        auto previous_index = terrain.get_index(previous_position);
        auto current_index = terrain.get_index(current_position);

        if (previous_index != current_index)
        {
            terrain.get_layer(Gecko::LayerType::Value::Traffic)->as<Gecko::TerrainLayerTraffic>()->substract(previous_index, 1);
            terrain.get_layer(Gecko::LayerType::Value::Traffic)->as<Gecko::TerrainLayerTraffic>()->add(current_index, 1);
        }
        */
    }

    void Citizen::update_orders(float time)
    {
        /*
        if (get_orders()->empty())
        {
            auto request_manager = Gecko::JobManager::getSingletonPtr();

            // If has any resources, try get unload job to drop them somewhere.
            auto jobs = request_manager->get_unload_job(get_id(), get_resources(), get_orders()->get_available());

            if (jobs.empty())
            {
                // If has no resources, try to get transport job.
                jobs = request_manager->get_transport_job(get_id(), get_resources(), get_orders()->get_available());

                if (jobs.empty())
                {
                    // If no job was assigned, wait.
                    jobs = std::vector<Gecko::Order*>
                    {
                        Gecko::OrderManager::getSingleton().order_wait(get_id(), get_id())
                    };
                }
            }

            for (const auto& job : jobs)
            {
                get_orders()->add_last(job);
            }
        }
        */

        base_type::update_orders(time);
    }

    void Citizen::update_resources(float time)
    {
        // TODO: Move resource efficiency production to engine.
        // Produce resources only if working.
        // if (is_working())
        {
            base_type::update_resources(time);
        }
    }

    Gecko::order_status Citizen::on_follow(Gecko::Order* order, float time)
    {
        /*
        TODO: Fix.
        auto follow_order = dynamic_cast<Gecko::OrderFollow*>(order);

        if (follow_order == nullptr)
        {
            L_WARNING << "Order does not exists or has invalid type.";

            return Gecko::order_status::fail;
        }

        auto& terrain = Gecko::Game::getSingleton().get_active_map()->get_terrain();
        auto& terrain_scale = terrain.get_scale();

        // Is order completed?
        auto object = Gecko::ObjectManager::getSingleton().get(follow_order->get_target_id());

        if (object == nullptr)
        {
            L_WARNING << "Cannot follow. Target " << follow_order->get_target_id() << " does not exist.";

            return Gecko::order_status::fail;
        }

        auto entrance = object->get_entrance();

        if (entrance.has_value() == false)
        {
            L_WARNING << "Cannot follow. Target " << follow_order->get_target_id() << " has no entrance.";

            on_order_retry(order);

            return Gecko::order_status::retry;
        }

        auto position = get_position();
        auto target_position = entrance->position;

        position.y = 0.0f;
        target_position.y = 0.0f;

        if (position.distance(target_position) < Settings::Game::DistanceComparisionMinValue)
        {
            return Gecko::order_status::complete_and_retry;
        }

        // Do we have everything needed to complete order?
        if (follow_order->get_points().size() <= 0)
        {
            auto points = terrain.get_path(position, target_position);

            if (points.size() == 0)
            {
                L_WARNING << "Cannot follow. Path from " << position << " to " << target_position << " is empty.";

                on_order_retry(order);

                return Gecko::order_status::retry;
            }

            follow_order->set_points(points);
            follow_order->push_front(position);
            follow_order->push_back(target_position);
        }

        // Try to complete order.
        float speed = 10.0f;

        // Get distance and direction to next point in path.
        auto distance = follow_order->get_points().front() - position;
        auto direction = distance.normalisedCopy();

        // Set direction.
        set_direction(direction);

        // Get distance that we can travel in this frame.
        auto next_distance = direction * speed * time;

        // If travel distance is greater than distance to next point in path,
        // move to next point in path. Otherwise, move to maximal distance.
        if (distance.squaredLength() > next_distance.squaredLength())
        {
            set_position(position + next_distance);
        }
        else
        {
            set_position(follow_order->get_points().front());

            follow_order->pop_front();
        }
        */

        return Gecko::order_status::in_progress;
    }

    Gecko::order_status Citizen::on_guard(Gecko::Order* order, float time)
    {
        /*
        TODO: Fix.
        // TODO: Implement guarding target.
        auto guard_order = dynamic_cast<Gecko::OrderGuard*>(order);

        if (guard_order == nullptr)
        {
            L_WARNING << "Order does not exists or has invalid type.";

            return Gecko::order_status::fail;
        }

        auto& terrain = Gecko::Game::getSingleton().get_active_map()->get_terrain();
        auto& terrain_scale = terrain.get_scale();

        // Is order completed?
        auto position = get_position();
        auto target_position = guard_order->get_target_position();

        if (guard_order->get_target_id().is_valid())
        {
            auto object = Gecko::ObjectManager::getSingleton().get(guard_order->get_target_id());

            if (object == nullptr)
            {
                L_WARNING << "Cannot guard. Target " << guard_order->get_target_id() << " does not exist.";

                return Gecko::order_status::fail;
            }

            auto entrance = object->get_entrance();

            if (entrance.has_value() == false)
            {
                L_WARNING << "Cannot guard. Target " << guard_order->get_target_id() << " has no entrance.";

                on_order_retry(order);

                return Gecko::order_status::retry;
            }

            target_position = entrance->position;
        }

        position.y = 0.0f;
        target_position.y = 0.0f;

        if (position.distance(target_position) < Settings::Game::DistanceComparisionMinValue)
        {
            return Gecko::order_status::complete_and_retry;
        }

        // Do we have everything needed to complete order?
        if (guard_order->get_points().size() <= 0)
        {
            auto points = terrain.get_path(position, target_position);

            if (points.size() == 0)
            {
                L_WARNING << "Cannot guard. Path from " << position << " to " << target_position << " is empty.";

                on_order_retry(order);

                return Gecko::order_status::retry;
            }

            guard_order->set_points(points);
            guard_order->push_front(position);
            guard_order->push_back(target_position);
        }

        // Try to complete order.
        float speed = 10.0f;

        // Get distance and direction to next point in path.
        auto distance = guard_order->get_points().front() - position;
        auto direction = distance.normalisedCopy();

        // Set direction.
        set_direction(direction);

        // Get distance that we can travel in this frame.
        auto next_distance = direction * speed * time;

        // If travel distance is greater than distance to next point in path,
        // move to next point in path. Otherwise, move to maximal distance.
        if (distance.squaredLength() > next_distance.squaredLength())
        {
            set_position(position + next_distance);
        }
        else
        {
            set_position(guard_order->get_points().front());

            guard_order->pop_front();
        }
        */

        return Gecko::order_status::in_progress;
    }

    Gecko::order_status Citizen::on_load(Gecko::Order* order, float time)
    {
        // Get order.
        auto load_order = dynamic_cast<Gecko::OrderLoad*>(order);

        if (load_order == nullptr)
        {
            L_WARNING << "Order does not exists or has invalid type.";

            return Gecko::order_status::fail;
        }

        // Check if target exists.
        auto target = Gecko::ObjectManager::getSingleton().get(load_order->get_target_id());

        if (target == nullptr)
        {
            L_WARNING << "Object " << load_order->get_target_id() << " does not exist.";

            return Gecko::order_status::fail;
        }

        // Check for distance to target object.
        if (Gecko::Utils::is_close_enough(get_position(), target->get_position(), Gecko::Settings::Game::ResourceLoadingMinDistance) == false)
        {
            L_WARNING << "Object " << load_order->get_target_id() << " is too far to load.";

            return Gecko::order_status::fail;
        }

        // Check if target has enough resources.
        auto resource_name = load_order->get_resource_name();
        auto resource_ordered = load_order->get_resource_value();

        // TODO: Remove (only for debugging now).
        if (target->get_resources()->has_resource(resource_name, resource_ordered) == false)
        {
            L_WARNING << "Object " << load_order->get_target_id() << " has no " << resource_ordered << " " << resource_name << ".";
        }

        auto resource_available_to_load = std::min(resource_ordered, target->get_resources()->get_current(resource_name));
        auto capacity_available_to_fill = std::min(resource_available_to_load, get_resources()->get_storage(resource_name));

        if (Gecko::Utils::is_enough_to_process(capacity_available_to_fill) == false)
        {
            L_WARNING << capacity_available_to_fill << " " << resource_name << " is not enough to load.";

            // on_order_retry(order);

            return Gecko::order_status::retry;
        }

        // Load resource.
        Gecko::Utils::move_resources(target->get_resources(), get_resources(), resource_name, capacity_available_to_fill);

        L_DEBUG << "Ordered: " << resource_ordered << ", available: " << resource_available_to_load << ", capacity:" << capacity_available_to_fill;

        return Gecko::order_status::complete;
    }

    Gecko::order_status Citizen::on_move(Gecko::Order* order, float time)
    {
        /*
        auto move_order = dynamic_cast<Gecko::OrderMove*>(order);

        if (move_order == nullptr)
        {
            L_WARNING << "Order does not exists or has invalid type.";

            return Gecko::order_status::fail;
        }

        // Is order completed?
        auto position = get_position();
        auto target_position = move_order->get_target_position();

        position.y = 0.0f;
        target_position.y = 0.0f;

        // Do we have everything needed to complete order?
        if (move_order->get_points().size() <= 0)
        {
            // TODO: Remove hardcoded value.
            auto layer = Gecko::Game::getSingleton().get_active_map()->get_layer("Terrain");
            auto search = Gecko::FollowRoadsSearch(layer);
            auto points = search.get_path(position, target_position);

            if (points)
            {
                // TODO: Move to path finding class.
                if (points.value().size() == 0)
                {
                    L_WARNING << "Cannot move. Path from " << position << " to " << target_position << " is empty.";

                    return Gecko::order_status::complete;
                }
            }
            else
            {
                L_WARNING << "Cannot move. Path from " << position << " to " << target_position << " does not exist.";

                on_order_retry(order);

                return Gecko::order_status::retry;
            }

            move_order->set_points(points.value());
        }

        // Get distance and direction to next point in path.
        auto distance = move_order->get_points().front() - position;
        auto direction = distance.normalisedCopy();

        // Set direction.
        set_direction(direction);

        // Get distance that we can travel in this frame.
        auto next_distance = direction * speed * time;

        // If travel distance is greater than distance to next point in path,
        // move to next point in path. Otherwise, move to maximal distance.
        if (distance.squaredLength() > next_distance.squaredLength())
        {
            set_position(position + next_distance);
        }
        else
        {
            set_position(move_order->get_points().front());

            move_order->pop_front();

            if (move_order->get_points().empty())
            {
                return Gecko::order_status::complete;
            }
        }
        */

        return Gecko::order_status::in_progress;

        /*
            Ogre::Vector3 position = get_position();
            position.y = 0.0f;
            Ogre::Vector3 nextPosition = position + next_distance;

            // TODO: Add function or data type to simplify conversion between float and int coordinates.
            //if (terrain.layer_type->get_data(nextPosition.x / terrain_scale.x, nextPosition.z / terrain_scale.z) == Gecko::TerrainLayerTypeType::Road)
            //{
                set_position(get_position() + next_distance);
                //distance_driven += next_distance.length();

                // Produce pollution.
                //float pollution = airPollutionProduction * time;

                // TODO: Refactor.
                //Gecko::ObjectManager::getSingleton().get<Terrain>()[0]->dataLayers[TerrainLayer::Type::AirPollution]->add(getPosition(), pollution);

                // TODO: Now, send only when vehicle passed through node center.
                // Send traffic data to layer.
                // TODO: Refactor.
                // map->get<Terrain>()[0]->dataLayers[TerrainLayer::Type::Traffic]->add(
                //     path.front(),
                //     1.0f,
                //     map->getClock().get_current()
                // );
            //}
            //else
            //{
            //    // Computed path is no longer available.
            //    path.clear();
            //}
        }
        else
        {
            Ogre::Vector3 position = get_position();
            position.y = 0.0f;
            Ogre::Vector3 nextPosition = position + distance;

            //if (terrain.layer_type->get_data(nextPosition.x / terrain_scale.x, nextPosition.z / terrain_scale.z) == Gecko::TerrainLayerTypeType::Road)
            //{
                Ogre::Vector3 position = get_position();
                position.y = 0.0f;
                set_position(position + distance);
                //distance_driven += distance.length();

                //// Produce pollution.
                //float pollution = airPollutionProduction * time;

                //// TODO: Refactor.
                //Gecko::ObjectManager::getSingleton().get<Terrain>()[0]->dataLayers[TerrainLayer::Type::AirPollution]->add(getPosition(), pollution);

                //// Send traffic data to layer.
                //// TODO: Refactor.
                //Gecko::ObjectManager::getSingleton().get<Terrain>()[0]->dataLayers[TerrainLayer::Type::Traffic]->add(
                //    path.front(),
                //    1.0f,
                //    map->getClock().get_current()
                //);

            //    path.pop_front();
            //}
            //else
            //{
            //    // Computed path is no longer available.
            //    path.clear();
            //}
        }
        */

        /*
        // Get path to target.
        if (path.size() == 0)
        {
            path = map->getPath(position, target);

            if (path.size() > 0)
            {
                setPosition(path.front());

                // TODO: Is this good fix?
                // TODO: It is needed, because last point in path is not equal to target, but it is shifted (car moves on the right side of the road).
                target = path.back();

                // Send data to data layer.
                // TODO: Fix.
                // TODO: Should we count also starting and ending point? Each start/end point very soon goes red.
                map->get<Terrain>()[0]->dataLayers[TerrainLayer::Type::Traffic]->add(
                    path.front(),
                    1.0f,
                    map->getClock().get_current()
                );
            }
        }
        */
    }

    Gecko::order_status Citizen::on_patrol(Gecko::Order* order, float time)
    {
        /*
        TODO: Fix.auto patrol_order = dynamic_cast<Gecko::OrderPatrol*>(order);

        if (patrol_order == nullptr)
        {
            L_WARNING << "Order does not exists or has invalid type.";

            return Gecko::order_status::fail;
        }

        auto& terrain = Gecko::Game::getSingleton().get_active_map()->get_terrain();
        auto& terrain_scale = terrain.get_scale();

        // Is order completed?
        auto position = get_position();
        auto target_position = patrol_order->get_target_position();

        if (patrol_order->get_target_id().is_valid())
        {
            auto object = Gecko::ObjectManager::getSingleton().get(patrol_order->get_target_id());

            if (object == nullptr)
            {
                L_WARNING << "Cannot patrol. Target " << patrol_order->get_target_id() << " does not exist.";

                return Gecko::order_status::fail;
            }

            auto entrance = object->get_entrance();

            if (entrance.has_value() == false)
            {
                L_WARNING << "Cannot patrol. Target " << patrol_order->get_target_id() << " has no entrance.";

                on_order_retry(order);

                return Gecko::order_status::retry;
            }

            target_position = entrance->position;
        }

        position.y = 0.0f;
        target_position.y = 0.0f;

        if (position.distance(target_position) < Settings::Game::DistanceComparisionMinValue)
        {
            return Gecko::order_status::complete_and_retry;
        }

        // Do we have everything needed to complete order?
        if (patrol_order->get_points().size() <= 0)
        {
            auto points = terrain.get_path(position, target_position);

            if (points.size() == 0)
            {
                L_WARNING << "Cannot patrol. Path from " << position << " to " << target_position << " is empty.";

                on_order_retry(order);

                return Gecko::order_status::retry;
            }

            patrol_order->set_points(points);
            patrol_order->push_front(position);
            patrol_order->push_back(target_position);
        }

        // Try to complete order.
        float speed = 10.0f;

        // Get distance and direction to next point in path.
        auto distance = patrol_order->get_points().front() - position;
        auto direction = distance.normalisedCopy();

        // Set direction.
        set_direction(direction);

        // Get distance that we can travel in this frame.
        auto next_distance = direction * speed * time;

        // If travel distance is greater than distance to next point in path,
        // move to next point in path. Otherwise, move to maximal distance.
        if (distance.squaredLength() > next_distance.squaredLength())
        {
            set_position(position + next_distance);
        }
        else
        {
            set_position(patrol_order->get_points().front());

            patrol_order->pop_front();
        }
        */

        return Gecko::order_status::in_progress;
    }

    Gecko::order_status Citizen::on_unload(Gecko::Order* order, float time)
    {
        // Get order.
        auto unload_order = dynamic_cast<Gecko::OrderUnload*>(order);

        if (unload_order == nullptr)
        {
            L_WARNING << "Order does not exists or has invalid type.";

            return Gecko::order_status::fail;
        }

        // Check if target exists.
        auto target = Gecko::ObjectManager::getSingleton().get(unload_order->get_target_id());

        if (target == nullptr)
        {
            L_WARNING << "Object " << unload_order->get_target_id() << " does not exist.";

            return Gecko::order_status::fail;
        }

        // Check for distance to target object.
        if (Gecko::Utils::is_close_enough(get_position(), target->get_position(), Gecko::Settings::Game::ResourceLoadingMinDistance) == false)
        {
            L_WARNING << "Object " << unload_order->get_target_id() << " is too far to unload.";

            return Gecko::order_status::fail;
        }

        // Check if target has enough storage.
        auto resource_name = unload_order->get_resource_name();
        auto resource_ordered = unload_order->get_resource_value();

        // TODO: Remove (only for debugging now).
        if (target->get_resources()->has_storage(resource_name, resource_ordered) == false)
        {
            L_WARNING << "Object " << unload_order->get_target_id() << " has no storage for " << resource_ordered << " " << resource_name << ".";
        }

        auto resource_available_to_unload = std::min(resource_ordered, get_resources()->get_current(resource_name));
        auto capacity_available_to_fill = std::min(resource_available_to_unload, target->get_resources()->get_storage(resource_name));

        if (Gecko::Utils::is_enough_to_process(capacity_available_to_fill) == false)
        {
            L_WARNING << capacity_available_to_fill << " " << resource_name << " is not enough to unload.";

            // on_order_retry(order);

            return Gecko::order_status::retry;
        }

        // Unload resource.
        Gecko::Utils::move_resources(get_resources(), target->get_resources(), resource_name, capacity_available_to_fill);

        L_DEBUG << "Ordered: " << resource_ordered << ", available: " << resource_available_to_unload << ", capacity:" << capacity_available_to_fill;

        return Gecko::order_status::complete;
    }
}
