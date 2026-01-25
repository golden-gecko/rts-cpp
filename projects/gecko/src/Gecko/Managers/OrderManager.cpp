#include "Gecko/Managers/OrderManager.hpp"

#include "Gecko/Managers/ObjectManager.hpp"
#include "Gecko/Objects/Object.hpp"
#include "Gecko/Orders/OrderCreate.hpp"
#include "Gecko/Orders/OrderFollow.hpp"
#include "Gecko/Orders/OrderGuard.hpp"
#include "Gecko/Orders/OrderLoad.hpp"
#include "Gecko/Orders/OrderMove.hpp"
#include "Gecko/Orders/OrderPatrol.hpp"
#include "Gecko/Orders/OrderUnload.hpp"
#include "Gecko/Orders/OrderWait.hpp"

Gecko::OrderManager* Ogre::Singleton<Gecko::OrderManager>::msSingleton = nullptr;

namespace Gecko
{
    void OrderManager::update(float time)
    {
        auto update = [](Order& order, float time)
        {
            order.update(time);
        };

        iterate(std::bind(update, std::placeholders::_1, time));
    }

    Order* OrderManager::order_attack(Id sender_id, Id receiver_id, Id target_id)
    {
        auto order = base_type::create("attack");

        order->init();
        order->set_sender_id(sender_id);
        order->set_receiver_id(receiver_id);

        return order;
    }

    Order* OrderManager::order_attack(Id sender_id, Id receiver_id, const Ogre::Vector3& target_position)
    {
        auto order = base_type::create("attack");

        order->init();
        order->set_sender_id(sender_id);
        order->set_receiver_id(receiver_id);

        return order;
    }

    Order* OrderManager::order_create(Id sender_id, Id receiver_id, const std::string& configuration_name, const Ogre::Vector3& position, Id player_id)
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

    Order* OrderManager::order_destroy(Id sender_id, Id receiver_id)
    {
        auto order = base_type::create("destroy");

        order->init();
        order->set_sender_id(sender_id);
        order->set_receiver_id(receiver_id);

        return order;
    }

    Order* OrderManager::order_disable(Id sender_id, Id receiver_id)
    {
        auto order = base_type::create("disable");

        order->init();
        order->set_sender_id(sender_id);
        order->set_receiver_id(receiver_id);

        return order;
    }

    Order* OrderManager::order_enable(Id sender_id, Id receiver_id)
    {
        auto order = base_type::create("enable");

        order->init();
        order->set_sender_id(sender_id);
        order->set_receiver_id(receiver_id);

        return order;
    }

    Order* OrderManager::order_follow(Id sender_id, Id receiver_id, Id target_id)
    {
        auto order = base_type::create("follow");

        order->init();
        order->set_sender_id(sender_id);
        order->set_receiver_id(receiver_id);

        auto order_follow = dynamic_cast<OrderFollow*>(order);

        order_follow->set_target_id(target_id);

        return order;
    }

    Order* OrderManager::order_guard(Id sender_id, Id receiver_id, Id target_id)
    {
        auto order = base_type::create("guard");

        order->init();
        order->set_sender_id(sender_id);
        order->set_receiver_id(receiver_id);

        auto order_guard = dynamic_cast<OrderGuard*>(order);

        order_guard->set_target_id(target_id);

        return order;
    }

    Order* OrderManager::order_guard(Id sender_id, Id receiver_id, const Ogre::Vector3& target_position)
    {
        auto order = base_type::create("guard");

        order->init();
        order->set_sender_id(sender_id);
        order->set_receiver_id(receiver_id);

        auto order_guard = dynamic_cast<OrderGuard*>(order);

        order_guard->set_target_position(target_position);

        return order;
    }

    Order* OrderManager::order_load(Id sender_id, Id receiver_id, Id target_id, const std::string& resource_name, float resource_value)
    {
        auto order = base_type::create("load");

        order->init();
        order->set_sender_id(sender_id);
        order->set_receiver_id(receiver_id);

        auto order_load = dynamic_cast<OrderLoad*>(order);

        order_load->set_target_id(target_id);
        order_load->set_resource_name(resource_name);
        order_load->set_resource_value(resource_value);

        return order;
    }

    Order* OrderManager::order_move(Id sender_id, Id receiver_id, Id target_id)
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

        // TODO: Refactor get_entrance to always return value.
        order_move->set_target_position(object->get_entrance().get_position());

        return order;
    }

    Order* OrderManager::order_move(Id sender_id, Id receiver_id, const Ogre::Vector3& target_position)
    {
        auto order = base_type::create("move");

        order->init();
        order->set_sender_id(sender_id);
        order->set_receiver_id(receiver_id);

        auto order_move = dynamic_cast<OrderMove*>(order);

        order_move->set_target_position(target_position);

        return order;
    }

    Order* OrderManager::order_patrol(Id sender_id, Id receiver_id, Id target_id)
    {
        auto order = base_type::create("patrol");

        order->init();
        order->set_sender_id(sender_id);
        order->set_receiver_id(receiver_id);

        auto order_patrol = dynamic_cast<OrderPatrol*>(order);

        order_patrol->set_target_id(target_id);

        return order;
    }

    Order* OrderManager::order_patrol(Id sender_id, Id receiver_id, const Ogre::Vector3& target_position)
    {
        auto order = base_type::create("patrol");

        order->init();
        order->set_sender_id(sender_id);
        order->set_receiver_id(receiver_id);

        auto order_patrol = dynamic_cast<OrderPatrol*>(order);

        order_patrol->set_target_position(target_position);

        return order;
    }

    Order* OrderManager::order_rally(Id sender_id, Id receiver_id, Id target_id)
    {
        auto order = base_type::create("rally");

        order->init();
        order->set_sender_id(sender_id);
        order->set_receiver_id(receiver_id);

        auto order_rally = dynamic_cast<OrderPatrol*>(order);

        order_rally->set_target_id(target_id);

        return order;
    }

    Order* OrderManager::order_rally(Id sender_id, Id receiver_id, const Ogre::Vector3& target_position)
    {
        auto order = base_type::create("rally");

        order->init();
        order->set_sender_id(sender_id);
        order->set_receiver_id(receiver_id);

        auto order_rally = dynamic_cast<OrderPatrol*>(order);

        order_rally->set_target_position(target_position);

        return order;
    }

    Order* OrderManager::order_stop(Id sender_id, Id receiver_id)
    {
        auto order = base_type::create("stop");

        order->init();
        order->set_sender_id(sender_id);
        order->set_receiver_id(receiver_id);

        return order;
    }

    Order* OrderManager::order_unload(Id sender_id, Id receiver_id, Id target_id, const std::string& resource_name, float resource_value)
    {
        auto order = base_type::create("unload");

        order->init();
        order->set_sender_id(sender_id);
        order->set_receiver_id(receiver_id);

        auto order_unload = dynamic_cast<OrderUnload*>(order);

        order_unload->set_target_id(target_id);
        order_unload->set_resource_name(resource_name);
        order_unload->set_resource_value(resource_value);

        return order;
    }

    Order* OrderManager::order_wait(Id sender_id, Id receiver_id, float time)
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
