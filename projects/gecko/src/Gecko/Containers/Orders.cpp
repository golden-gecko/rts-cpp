#include "Gecko/Containers/Orders.hpp"

#include "Gecko/Configuration.hpp"
#include "Gecko/Exception.hpp"
#include "Gecko/Log.hpp"
#include "Gecko/Managers/OrderManager.hpp"
#include "Gecko/Orders/Order.hpp"

namespace Gecko
{
    Orders::~Orders()
    {
        remove_all_orders();
    }

    std::shared_ptr<Configuration> Orders::serialize() const
    {
        auto configuration = std::make_shared<Configuration>();

        for (const auto& i : available)
        {
            configuration->append("available", order_type::to_string(i));
        }

        for (const auto& i : queue)
        {
            auto order = OrderManager::getSingleton().get(i);

            if (order)
            {
                configuration->append("queue", order->serialize());
            }
        }

        return configuration;
    }

    void Orders::deserialize(const std::shared_ptr<Configuration>& configuration)
    {
        clear();

        // TODO: Move to method?
        available.clear();

        if (configuration->has_member("available"))
        {
            auto child = configuration->get_child("available");

            for (const auto& i : *(child))
            {
                available.emplace(order_type::from_string(i.asString()));
            }
        }

        if (configuration->has_member("queue"))
        {
            auto child = configuration->get_child("queue");

            for (const auto& i : *(child))
            {
                auto order_configuration = std::make_shared<Configuration>(i);
                auto configuration = order_configuration->get_string("configuration");
                auto order = OrderManager::getSingleton().create(configuration);

                if (order == nullptr)
                {
                    throw Exception("Failed to create '" + configuration + "' order.");
                }

                order->deserialize(order_configuration);

                queue.emplace_back(order->get_id());
            }
        }
    }

    bool Orders::add_first(Id order_id)
    {
        auto order = OrderManager::getSingleton().get(order_id);

        if (order == nullptr)
        {
            // L_WARNING << "Order '" << order_id << "' not exists.";

            return false;
        }

        if (is_available(order->get_type()) == false)
        {
            // L_WARNING << "Cannot add order " << order_type::to_string(order->get_type()) << ".";

            OrderManager::getSingleton().destroy(order_id);

            return false;
        }

        queue.emplace_front(order_id);

        return true;
    }

    bool Orders::add_last(Id order_id)
    {
        auto order = OrderManager::getSingleton().get(order_id);

        if (order == nullptr)
        {
            // L_WARNING << "Order '" << order_id << "' not exists.";

            return false;
        }

        if (is_available(order->get_type()) == false)
        {
            // L_WARNING << "Cannot add order '" << order_type::to_string(order->get_type()) << "'.";

            OrderManager::getSingleton().destroy(order_id);

            return false;
        }

        queue.emplace_back(order_id);

        return true;
    }

    void Orders::remove(Id order_id)
    {
        auto i = std::ranges::find(queue, order_id);

        if (i != queue.end())
        {
            OrderManager::getSingleton().destroy(order_id);

            queue.erase(i);
        }
    }

    void Orders::remove_current_order()
    {
        if (queue.empty() == false)
        {
            OrderManager::getSingleton().destroy(queue.front());

            queue.pop_front();
        }
    }

    void Orders::remove_all_orders()
    {
        if (OrderManager::getSingletonPtr())
        {
            for (const auto& order_id : queue)
            {
                OrderManager::getSingleton().destroy(order_id);
            }
        }

        queue.clear();
    }

    void Orders::move_first_to_end()
    {
        auto& order_id = queue.front();

        queue.pop_front();
        queue.push_back(order_id);
    }
}
