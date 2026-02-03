#include "Gecko/Objects/Vehicle.hpp"

#include "Gecko/Components/Storage.hpp"
#include "Gecko/Containers/Orders.hpp"
#include "Gecko/Containers/Resources.hpp"
#include "Gecko/DataLayers/DataLayer.hpp"
#include "Gecko/Layers/Layer.hpp"
#include "Gecko/Log.hpp"
#include "Gecko/Managers/ObjectManager.hpp"
#include "Gecko/Managers/OrderManager.hpp"
#include "Gecko/Maps/Map.hpp"
#include "Gecko/Navigation/Search/DirectLineSearch.hpp"
#include "Gecko/Orders/OrderAttack.hpp"
#include "Gecko/Orders/OrderLoad.hpp"
#include "Gecko/Orders/OrderMove.hpp"
#include "Gecko/Orders/OrderUnload.hpp"
#include "Gecko/Settings.hpp"
#include "Gecko/Utils/Utils.hpp"

namespace Gecko
{
    Vehicle* Vehicle::create(Vehicle* memory, const ConfigurationPtr& configuration)
    {
        auto map = new (memory) Vehicle();

        map->deserialize(configuration);

        return map;
    }

    OrderStatus Vehicle::on_attack(Order* order, float time)
    {
        // Get order.
        auto attack_order = dynamic_cast<OrderAttack*>(order);

        if (attack_order == nullptr)
        {
            L_WARNING << "Order not exists or has invalid type.";

            return OrderStatus::fail;
        }

        // Find weapons and assign target to them.
        // TODO: Implement.
        // for (const auto& i : *(get_components()))

        return OrderStatus::complete;
    }

    OrderStatus Vehicle::on_load(Order* order, float time)
    {
        // Get order.
        auto load_order = dynamic_cast<OrderLoad*>(order);

        if (load_order == nullptr)
        {
            L_WARNING << "Order not exists or has invalid type.";

            return OrderStatus::fail;
        }

        // Check if target exists.
        auto target = ObjectManager::getSingleton().get(load_order->get_target_id());

        if (target == nullptr)
        {
            L_WARNING << "Object " << load_order->get_target_id() << " does not exist.";

            return OrderStatus::fail;
        }

        // Check for distance to target object.
        if (Utils::is_close_enough(get_position(), target->get_position(), Settings::Game::ResourceLoadingMinDistance) == false)
        {
            L_WARNING << "Object " << load_order->get_target_id() << " is too far to load.";

            return OrderStatus::fail;
        }

        // Check if target has enough resources.
        auto resource_name = load_order->get_resource_name();
        auto resource_ordered = load_order->get_resource_value();

        if (target->get_resources()->has_resource(resource_name, resource_ordered) == false)
        {
            L_WARNING << "Object " << load_order->get_target_id() << " has no " << resource_ordered << " " << resource_name << ".";
        }

        auto resource_available_to_load = std::min(resource_ordered, target->get_resources()->get_current(resource_name));
        auto capacity_available_to_fill = std::min(resource_available_to_load, get_resources()->get_storage(resource_name));

        if (Utils::is_enough_to_process(capacity_available_to_fill) == false)
        {
            L_WARNING << capacity_available_to_fill << " " << resource_name << " is not enough to load.";

            return OrderStatus::retry;
        }

        // Load resource.
        Utils::move_resources(target->get_resources(), get_resources(), resource_name, capacity_available_to_fill);

        L_DEBUG << "Ordered: " << resource_ordered << ", available: " << resource_available_to_load << ", capacity:" << capacity_available_to_fill;

        return OrderStatus::complete;
    }

    OrderStatus Vehicle::on_move(Order* order, float time)
    {
        auto move_order = dynamic_cast<OrderMove*>(order);

        if (move_order == nullptr)
        {
            L_WARNING << "Order not exists or has invalid type.";

            return OrderStatus::fail;
        }

        // Is order completed?
        auto position = get_position();
        auto target_position = move_order->get_target_position();

        position.y = 0.0f;
        target_position.y = 0.0f;

        // TODO: Hardcoded layer name.
        const auto& layer = get_owner()->get_layer("Terrain");

        // Do we have everything needed to complete order?
        if (move_order->get_path().get_points().empty())
        {
            auto points = layer->search(position, target_position);

            if (points)
            {
                // TODO: Move to path finding class.
                if (points.value().empty())
                {
                    L_WARNING << "Cannot move. Path from " << position << " to " << target_position << " is empty.";

                    return OrderStatus::complete;
                }
            }
            else
            {
                L_WARNING << "Cannot move. Path from " << position << " to " << target_position << " does not exist.";

                return OrderStatus::retry;
            }

            move_order->get_path().set_points(points.value());
        }

        // Get distance and direction to next point in path.
        auto front = move_order->get_path().get_points().front();
        auto next_point = layer->get_position(front.x * layer->get_scale().x, front.z * layer->get_scale().z);
        next_point.y = 0.0f;

        auto distance = next_point - position;
        auto direction = distance.normalisedCopy();

        // Set direction.
        set_direction(direction);

        // TODO: Get speed from Drive component.
        // Get distance that we can travel in this frame.
        auto next_distance = direction * 10.0f /* speed */ * time;

        // TODO: Hardcoded.
        auto navigation_layer = layer->get_data_layer("Navigation");

        // If travel distance is greater than distance to next point in path,
        // move to next point in path. Otherwise, move to max distance.
        if (distance.squaredLength() > next_distance.squaredLength())
        {
            // TODO: Copy to "else".
            auto current_index = layer->get_index(position);
            auto next_index = layer->get_index(position + next_distance);

            if (current_index != next_index && navigation_layer->get_data(next_index.x, next_index.z) > 0)
            {
                return OrderStatus::in_progress;
            }
            else
            {
                set_position(position + next_distance, false);
            }
        }
        else
        {
            auto front = move_order->get_points().front();
            set_position(layer->get_position(front.x * layer->get_scale().x, front.z * layer->get_scale().z));

            move_order->get_path().get_points().pop_front();

            if (move_order->get_points().empty())
            {
                return OrderStatus::complete;
            }
        }

        return OrderStatus::in_progress;
    }

    OrderStatus Vehicle::on_unload(Order* order, float time)
    {
        // Get order.
        auto unload_order = dynamic_cast<OrderUnload*>(order);

        if (unload_order == nullptr)
        {
            L_WARNING << "Order not exists or has invalid type.";

            return OrderStatus::fail;
        }

        // Check if target exists.
        auto target = ObjectManager::getSingleton().get(unload_order->get_target_id());

        if (target == nullptr)
        {
            L_WARNING << "Object " << unload_order->get_target_id() << " does not exist.";

            return OrderStatus::fail;
        }

        // Check for distance to target object.
        if (Utils::is_close_enough(get_position(), target->get_position(), Settings::Game::ResourceLoadingMinDistance) == false)
        {
            L_WARNING << "Object " << unload_order->get_target_id() << " is too far to unload.";

            return OrderStatus::fail;
        }

        // Check if target has enough storage.
        auto resource_name = unload_order->get_resource_name();
        auto resource_ordered = unload_order->get_resource_value();

        if (target->get_resources()->has_storage(resource_name, resource_ordered) == false)
        {
            L_WARNING << "Object " << unload_order->get_target_id() << " has no storage for " << resource_ordered << " " << resource_name << ".";
        }

        auto resource_available_to_unload = std::min(resource_ordered, get_resources()->get_current(resource_name));
        auto capacity_available_to_fill = std::min(resource_available_to_unload, target->get_resources()->get_storage(resource_name));

        if (Utils::is_enough_to_process(capacity_available_to_fill) == false)
        {
            L_WARNING << capacity_available_to_fill << " " << resource_name << " is not enough to unload.";

            return OrderStatus::retry;
        }

        // Unload resource.
        Utils::move_resources(get_resources(), target->get_resources(), resource_name, capacity_available_to_fill);

        L_DEBUG << "Ordered: " << resource_ordered << ", available: " << resource_available_to_unload << ", capacity:" << capacity_available_to_fill;

        return OrderStatus::complete;
    }
}
