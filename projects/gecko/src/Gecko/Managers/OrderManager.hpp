#pragma once

#include "Gecko/Id.hpp"
#include "Gecko/Interfaces/Updatable.hpp"
#include "Gecko/Managers/Manager.hpp"
#include "Gecko/Settings.hpp"

namespace Gecko
{
    class OrderManager :
        public Ogre::Singleton<OrderManager>,
        public Manager<Order, std::string, Id>,
        public Updatable
    {
    public:
        using base_type = Manager<Order, std::string, Id>;

    public:
        void update(float time) override;

    public:
        Order* order_attack(Id sender_id, Id receiver_id, Id target_id);
        Order* order_attack(Id sender_id, Id receiver_id, const Ogre::Vector3& target_position);

        Order* order_create(Id sender_id, Id receiver_id, const std::string& configuration_name, const Ogre::Vector3& position, Id player_id);

        Order* order_destroy(Id sender_id, Id receiver_id);

        Order* order_disable(Id sender_id, Id receiver_id);

        Order* order_enable(Id sender_id, Id receiver_id);

        Order* order_follow(Id sender_id, Id receiver_id, Id target_id);

        Order* order_guard(Id sender_id, Id receiver_id, Id target_id);
        Order* order_guard(Id sender_id, Id receiver_id, const Ogre::Vector3& target_position);

        Order* order_load(Id sender_id, Id receiver_id, Id target_id, const std::string& resource_name, float resource_value);

        Order* order_move(Id sender_id, Id receiver_id, Id target_id);
        Order* order_move(Id sender_id, Id receiver_id, const Ogre::Vector3& target_position);

        Order* order_patrol(Id sender_id, Id receiver_id, Id target_id);
        Order* order_patrol(Id sender_id, Id receiver_id, const Ogre::Vector3& target_position);

        Order* order_rally(Id sender_id, Id receiver_id, Id target_id);
        Order* order_rally(Id sender_id, Id receiver_id, const Ogre::Vector3& target_position);

        Order* order_stop(Id sender_id, Id receiver_id);

        Order* order_unload(Id sender_id, Id receiver_id, Id target_id, const std::string& resource_name, float resource_value);

        Order* order_wait(Id sender_id, Id receiver_id, float time = Settings::Game::OrderWaitTime);
    };
}
