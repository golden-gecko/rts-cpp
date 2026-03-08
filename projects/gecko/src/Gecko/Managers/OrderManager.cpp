#include "Gecko/Managers/OrderManager.hpp"

#include "Gecko/Configuration.hpp"
#include "Gecko/Log.hpp"
#include "Gecko/Managers/ConfigurationManager.hpp"
#include "Gecko/Managers/ObjectManager.hpp"
#include "Gecko/Objects/Object.hpp"
#include "Gecko/Orders/OrderAttack.hpp"
#include "Gecko/Orders/OrderCreate.hpp"
#include "Gecko/Orders/OrderDestroy.hpp"
#include "Gecko/Orders/OrderFollow.hpp"
#include "Gecko/Orders/OrderGuard.hpp"
#include "Gecko/Orders/OrderLoad.hpp"
#include "Gecko/Orders/OrderMove.hpp"
#include "Gecko/Orders/OrderPatrol.hpp"
#include "Gecko/Orders/OrderRally.hpp"
#include "Gecko/Orders/OrderStop.hpp"
#include "Gecko/Orders/OrderUnload.hpp"
#include "Gecko/Orders/OrderWait.hpp"
#include "Gecko/Timer.hpp"
#include "Gecko/Utils/Time.hpp"

Gecko::OrderManager* Ogre::Singleton<Gecko::OrderManager>::msSingleton = nullptr;

namespace Gecko
{
    void OrderManager::init()
    {
        L_TIME("OrderManager::init()");

        std::uint64_t max_size = m_configuration->get_int<std::uint64_t>("memory.orders");

        for (const auto& [name, configuration] : ConfigurationManager::getSingleton())
        {
            L_INFO << "Loading '" << name << "' configuration.";

            std::string type = configuration->get_string("type", "");

            if (type == "Attack")
            {
                register_type<OrderAttack>(name, std::bind(OrderAttack::create, std::placeholders::_1, configuration));
            }
            else if (type == "Create")
            {
                register_type<OrderCreate>(name, std::bind(OrderCreate::create, std::placeholders::_1, configuration));
            }
            else if (type == "Destroy")
            {
                register_type<OrderDestroy>(name, std::bind(OrderDestroy::create, std::placeholders::_1, configuration));
            }
            else if (type == "Follow")
            {
                register_type<OrderFollow>(name, std::bind(OrderFollow::create, std::placeholders::_1, configuration));
            }
            else if (type == "Guard")
            {
                register_type<OrderGuard>(name, std::bind(OrderGuard::create, std::placeholders::_1, configuration));
            }
            else if (type == "Load")
            {
                register_type<OrderLoad>(name, std::bind(OrderLoad::create, std::placeholders::_1, configuration));
            }
            else if (type == "Move")
            {
                register_type<OrderMove>(name, std::bind(OrderMove::create, std::placeholders::_1, configuration));
            }
            else if (type == "Patrol")
            {
                register_type<OrderPatrol>(name, std::bind(OrderPatrol::create, std::placeholders::_1, configuration));
            }
            else if (type == "Rally")
            {
                register_type<OrderRally>(name, std::bind(OrderRally::create, std::placeholders::_1, configuration));
            }
            else if (type == "Stop")
            {
                register_type<OrderStop>(name, std::bind(OrderStop::create, std::placeholders::_1, configuration));
            }
            else if (type == "Unload")
            {
                register_type<OrderUnload>(name, std::bind(OrderUnload::create, std::placeholders::_1, configuration));
            }
            else if (type == "Wait")
            {
                register_type<OrderWait>(name, std::bind(OrderWait::create, std::placeholders::_1, configuration));
            }

            allocate(name, max_size);
        }
    }

    void OrderManager::deinit()
    {
        unregister_all();
    }

    void OrderManager::update(float time)
    {
        auto update = [time](Order& order)
        {
            order.update(time);
        };

        iterate(std::bind(update, std::placeholders::_1));
    }

    OrderManager::OrderManager(const ConfigurationPtr& configuration) :
        m_configuration(configuration)
    {
    }

    OrderPtr OrderManager::order_attack(const Id& sender_id, const Id& receiver_id, const Id& target_id)
    {
        auto order = base_type::create("attack");

        order->init();
        order->set_sender_id(sender_id);
        order->set_receiver_id(receiver_id);

        auto order_create = dynamic_cast<OrderAttack*>(order);

        order_create->set_target_id(target_id);

        return order;
    }

    OrderPtr OrderManager::order_attack(const Id& sender_id, const Id& receiver_id, const Ogre::Vector3& target_position)
    {
        auto order = base_type::create("attack");

        order->init();
        order->set_sender_id(sender_id);
        order->set_receiver_id(receiver_id);

        auto order_create = dynamic_cast<OrderAttack*>(order);

        order_create->set_target_position(target_position);

        return order;
    }

    OrderPtr OrderManager::order_create(const Id& sender_id, const Id& receiver_id, const std::string& configuration_name, const Ogre::Vector3& position, const Id& player_id)
    {
        auto order = base_type::create("create");

        order->init();
        order->set_sender_id(sender_id);
        order->set_receiver_id(receiver_id);

        auto order_create = dynamic_cast<OrderCreate*>(order);

        order_create->set_player_id(player_id);
        order_create->set_configuration_name(configuration_name);
        order_create->set_position(position);

        return order;
    }

    OrderPtr OrderManager::order_destroy(const Id& sender_id, const Id& receiver_id)
    {
        auto order = base_type::create("destroy");

        order->init();
        order->set_sender_id(sender_id);
        order->set_receiver_id(receiver_id);

        return order;
    }

    OrderPtr OrderManager::order_disable(const Id& sender_id, const Id& receiver_id)
    {
        auto order = base_type::create("disable");

        order->init();
        order->set_sender_id(sender_id);
        order->set_receiver_id(receiver_id);

        return order;
    }

    OrderPtr OrderManager::order_enable(const Id& sender_id, const Id& receiver_id)
    {
        auto order = base_type::create("enable");

        order->init();
        order->set_sender_id(sender_id);
        order->set_receiver_id(receiver_id);

        return order;
    }

    OrderPtr OrderManager::order_follow(const Id& sender_id, const Id& receiver_id, const Id& target_id)
    {
        auto order = base_type::create("follow");

        order->init();
        order->set_sender_id(sender_id);
        order->set_receiver_id(receiver_id);

        auto order_follow = dynamic_cast<OrderFollow*>(order);

        order_follow->set_target_id(target_id);

        return order;
    }

    OrderPtr OrderManager::order_guard(const Id& sender_id, const Id& receiver_id, const Id& target_id)
    {
        auto order = base_type::create("guard");

        order->init();
        order->set_sender_id(sender_id);
        order->set_receiver_id(receiver_id);

        auto order_guard = dynamic_cast<OrderGuard*>(order);

        order_guard->set_target_id(target_id);

        return order;
    }

    OrderPtr OrderManager::order_guard(const Id& sender_id, const Id& receiver_id, const Ogre::Vector3& target_position)
    {
        auto order = base_type::create("guard");

        order->init();
        order->set_sender_id(sender_id);
        order->set_receiver_id(receiver_id);

        auto order_guard = dynamic_cast<OrderGuard*>(order);

        order_guard->set_target_position(target_position);

        return order;
    }

    OrderPtr OrderManager::order_load(const Id& sender_id, const Id& receiver_id, const Id& target_id, const std::string& resource_name, float resource_value, float time)
    {
        auto order = base_type::create("load");

        order->init();
        order->set_sender_id(sender_id);
        order->set_receiver_id(receiver_id);

        auto order_load = dynamic_cast<OrderLoad*>(order);

        order_load->set_target_id(target_id);
        order_load->set_resource_name(resource_name);
        order_load->set_resource_value(resource_value);

        order_load->get_load_timer().set_max(time);

        return order;
    }

    OrderPtr OrderManager::order_move(const Id& sender_id, const Id& receiver_id, const Id& target_id)
    {
        auto object = ObjectManager::getSingleton().get(target_id);

        if (object == nullptr)
        {
            return nullptr;
        }

        auto order = base_type::create("move");

        order->init();
        order->set_sender_id(sender_id);
        order->set_receiver_id(receiver_id);

        auto order_move = dynamic_cast<OrderMove*>(order);

        order_move->set_target_position(object->get_entrance().get_position());

        return order;
    }

    OrderPtr OrderManager::order_move(const Id& sender_id, const Id& receiver_id, const Ogre::Vector3& target_position)
    {
        auto order = base_type::create("move");

        order->init();
        order->set_sender_id(sender_id);
        order->set_receiver_id(receiver_id);

        auto order_move = dynamic_cast<OrderMove*>(order);

        order_move->set_target_position(target_position);

        return order;
    }

    OrderPtr OrderManager::order_patrol(const Id& sender_id, const Id& receiver_id, const Id& target_id)
    {
        auto order = base_type::create("patrol");

        order->init();
        order->set_sender_id(sender_id);
        order->set_receiver_id(receiver_id);

        auto order_patrol = dynamic_cast<OrderPatrol*>(order);

        order_patrol->set_target_id(target_id);

        return order;
    }

    OrderPtr OrderManager::order_patrol(const Id& sender_id, const Id& receiver_id, const Ogre::Vector3& target_position)
    {
        auto order = base_type::create("patrol");

        order->init();
        order->set_sender_id(sender_id);
        order->set_receiver_id(receiver_id);

        auto order_patrol = dynamic_cast<OrderPatrol*>(order);

        order_patrol->set_target_position(target_position);

        return order;
    }

    OrderPtr OrderManager::order_rally(const Id& sender_id, const Id& receiver_id, const Id& target_id)
    {
        auto order = base_type::create("rally");

        order->init();
        order->set_sender_id(sender_id);
        order->set_receiver_id(receiver_id);

        auto order_rally = dynamic_cast<OrderPatrol*>(order);

        order_rally->set_target_id(target_id);

        return order;
    }

    OrderPtr OrderManager::order_rally(const Id& sender_id, const Id& receiver_id, const Ogre::Vector3& target_position)
    {
        auto order = base_type::create("rally");

        order->init();
        order->set_sender_id(sender_id);
        order->set_receiver_id(receiver_id);

        auto order_rally = dynamic_cast<OrderPatrol*>(order);

        order_rally->set_target_position(target_position);

        return order;
    }

    OrderPtr OrderManager::order_stop(const Id& sender_id, const Id& receiver_id)
    {
        auto order = base_type::create("stop");

        order->init();
        order->set_sender_id(sender_id);
        order->set_receiver_id(receiver_id);

        return order;
    }

    OrderPtr OrderManager::order_unload(const Id& sender_id, const Id& receiver_id, const Id& target_id, const std::string& resource_name, float resource_value, float time)
    {
        auto order = base_type::create("unload");

        order->init();
        order->set_sender_id(sender_id);
        order->set_receiver_id(receiver_id);

        auto order_unload = dynamic_cast<OrderUnload*>(order);

        order_unload->set_target_id(target_id);
        order_unload->set_resource_name(resource_name);
        order_unload->set_resource_value(resource_value);

        order_unload->get_unload_timer().set_max(time);

        return order;
    }

    OrderPtr OrderManager::order_wait(const Id& sender_id, const Id& receiver_id, float time)
    {
        auto order = base_type::create("wait");

        order->init();
        order->set_sender_id(sender_id);
        order->set_receiver_id(receiver_id);

        auto order_move = dynamic_cast<OrderWait*>(order);

        order_move->get_wait_timer().set_max(time);

        return order;
    }
}
