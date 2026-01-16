#include "Gecko/Managers/JobManager.hpp"

#include "Gecko/Components/Drive.hpp"
#include "Gecko/Components/Storage.hpp"
#include "Gecko/Components/Weapon.hpp"
#include "Gecko/Containers/Components.hpp"
#include "Gecko/Containers/Resources.hpp"
#include "Gecko/Managers/ObjectManager.hpp"
#include "Gecko/Managers/OrderManager.hpp"
#include "Gecko/Utils/Utils.hpp"

Gecko::JobManager* Ogre::Singleton<Gecko::JobManager>::msSingleton = nullptr;

namespace Gecko
{
    Request::Request(const Id& requester_id, const std::string& resource_name, float resource_value, float resource_priority) :
        requester_id(requester_id),
        resource_name(resource_name),
        resource_value(resource_value),
        resource_priority(resource_priority)
    {
    }

    void JobManager::add_in(const Id& requester, const std::string& resource_name, float resource_value, float resource_priority)
    {
        // If requester is in the queue, update resource value.
        // Otherwise, add requester to the end of queue.
        auto is_queued = std::ranges::find_if(m_in_queue,
            [&](const auto& x)
            {
                return x.requester_id == requester && x.resource_name == resource_name;
            }
        );

        if (is_queued == m_in_queue.end())
        {
            auto i = std::ranges::find_if(m_in_queue,
                [&](const auto& x)
                {
                    return x.resource_priority < resource_priority;
                }
            );

            m_in_queue.emplace(i, Request(requester, resource_name, resource_value, resource_priority));
        }
        else
        {
            is_queued->resource_value = resource_value;
        }
    }

    void JobManager::add_out(const Id& requester, const std::string& resource_name, float resource_value, float resource_priority)
    {
        // If requester is in the queue, update resource value.
        // Otherwise, add requester to the end of queue.
        auto is_queued = std::ranges::find_if(m_out_queue,
            [&](const auto& x)
            {
                return x.requester_id == requester && x.resource_name == resource_name;
            }
        );

        if (is_queued == m_out_queue.end())
        {
            auto i = std::ranges::find_if(m_out_queue,
                [&](const auto& x)
                {
                    return x.resource_priority < resource_priority;
                }
            );

            m_out_queue.emplace(i, Request(requester, resource_name, resource_value, resource_priority));
        }
        else
        {
            is_queued->resource_value = resource_value;
        }
    }

    std::vector<Order*> JobManager::get_job(const Id& id, const std::shared_ptr<Components>& components, const std::shared_ptr<Resources>& resources)
    {
        // Priorities:
        // 1. Attack.
        // 2. Heal or Repair.
        // 3. Build.
        // 4. Transport.

        if (components->has<Weapon>())
        {
            auto jobs = get_attack_job(id, resources);

            if (jobs.empty() == false)
            {
                return jobs;
            }
        }
        else if (components->has<Drive>() && components->has<Storage>())
        {
            auto jobs = get_unload_job(id, resources);

            if (jobs.empty() == false)
            {
                return jobs;
            }

            jobs = get_transport_job(id, resources);

            if (jobs.empty() == false)
            {
                return jobs;
            }
        }

        return {};
    }

    void JobManager::remove_in(const Id& requester, const std::string& resource_name)
    {
        std::erase_if(m_in_queue,
            [&](const auto& x)
            {
                return x.requester_id == requester && x.resource_name == resource_name;
            }
        );
    }

    void JobManager::remove_out(const Id& requester, const std::string& resource_name)
    {
        std::erase_if(m_out_queue,
            [&](const auto& x)
            {
                return x.requester_id == requester && x.resource_name == resource_name;
            }
        );
    }

    std::vector<Order*> JobManager::get_attack_job(const Id& id, const std::shared_ptr<Resources>& resources)
    {
        // TODO: Implement.
        /*
        auto& object_manager = ObjectManager::getSingleton();
        auto& order_manager = OrderManager::getSingleton();

        auto object = object_manager.get_in_range();

        return {
            // TODO: Replace with transport order.
            // TODO: Remove wait orders (only for debbuging).
            order_manager.order_wait(id, id, 0.1f),
            order_manager.order_attack(id, id, 0.1f),
            order_manager.order_wait(id, id, 0.1f)
        };
        */

        return {};
    }

    std::vector<Order*> JobManager::get_transport_job(const Id& id, const std::shared_ptr<Resources>& resources)
    {
        auto order_manager = OrderManager::getSingletonPtr();

        // Start with output resources, because if there is not output resources then there is nothing to transport.
        for (auto out_request = m_out_queue.cbegin(); out_request != m_out_queue.cend(); ++out_request)
        {
            // TODO: Optimize.
            for (auto in_request = m_in_queue.cbegin(); in_request != m_in_queue.cend(); ++in_request)
            {
                // Check if output and input requesters are the same.
                if (out_request->requester_id == in_request->requester_id)
                {
                    continue;
                }

                // Match resource input with resource output.
                if (out_request->resource_name != in_request->resource_name)
                {
                    continue;
                }

                // Check if object can carry input resource.
                auto storage = resources->get_storage(in_request->resource_name);

                if (Utils::is_enough_to_process(storage) == false)
                {
                    continue;
                }

                // Create orders:
                // - move to output requester,
                // - load resources, take only that much as input requester can store,
                // - move to input requester,
                // - unload resources.
                // TODO: Maybe we could ask input requester for current storage?
                auto jobs = {
                    // TODO: Replace with transport order.
                    // TODO: Remove wait orders (only for debbuging).
                    order_manager->order_wait(id, id, 0.1f),
                    order_manager->order_move(id, id, out_request->requester_id),
                    order_manager->order_wait(id, id, 0.1f),
                    order_manager->order_load(id, id, out_request->requester_id, in_request->resource_name, in_request->resource_value),
                    order_manager->order_wait(id, id, 0.1f),
                    order_manager->order_move(id, id, in_request->requester_id),
                    order_manager->order_wait(id, id, 0.1f),
                    order_manager->order_unload(id, id, in_request->requester_id, in_request->resource_name, in_request->resource_value),
                    order_manager->order_wait(id, id, 0.1f)
                };

                // TODO: Remove when orders are completed.
                // Remove input and output requesters from queues.
                m_out_queue.erase(out_request);
                m_in_queue.erase(in_request);

                return jobs;
            }
        }

        return {};
    }

    std::vector<Order*> JobManager::get_unload_job(const Id& id, const std::shared_ptr<Resources>& resources)
    {
        auto order_manager = OrderManager::getSingletonPtr();

        for (auto in_request = m_in_queue.cbegin(); in_request != m_in_queue.cend(); ++in_request)
        {
            // Check if resource is carried by object.
            if (Utils::is_enough_to_process(resources->get_current(in_request->resource_name)) == false)
            {
                continue;
            }

            // Create orders.
            auto jobs = {
                // TODO: Remove wait orders (only for debbuging).
                order_manager->order_wait(id, id, 0.1f),
                order_manager->order_move(id, id, in_request->requester_id),
                order_manager->order_wait(id, id, 0.1f),
                order_manager->order_unload(id, id, in_request->requester_id, in_request->resource_name, in_request->resource_value),
                order_manager->order_wait(id, id, 0.1f)
            };

            // TODO: Remove when orders are completed.
            // Remove requester from queue.
            m_in_queue.erase(in_request);

            return jobs;
        }

        return {};
    }
}
