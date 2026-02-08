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
    private:
        using base_type = Manager<Order, std::string, Id>;

    public:
        // From Updatable.
        void update(float time) override;

    public:
        void init(const ConfigurationPtr& configuration);
        void deinit();

        Order* order_attack(const Id& sender_id, const Id& receiver_id, const Id& target_id);
        Order* order_attack(const Id& sender_id, const Id& receiver_id, const Ogre::Vector3& target_position);

        Order* order_create(const Id& sender_id, const Id& receiver_id, const std::string& configuration_name, const Ogre::Vector3& position, const Id& player_id);

        Order* order_destroy(const Id& sender_id, const Id& receiver_id);

        Order* order_disable(const Id& sender_id, const Id& receiver_id);

        Order* order_enable(const Id& sender_id, const Id& receiver_id);

        Order* order_follow(const Id& sender_id, const Id& receiver_id, const Id& target_id);

        Order* order_guard(const Id& sender_id, const Id& receiver_id, const Id& target_id);
        Order* order_guard(const Id& sender_id, const Id& receiver_id, const Ogre::Vector3& target_position);

        Order* order_load(const Id& sender_id, const Id& receiver_id, const Id& target_id, const std::string& resource_name, float resource_value);

        Order* order_move(const Id& sender_id, const Id& receiver_id, const Id& target_id);
        Order* order_move(const Id& sender_id, const Id& receiver_id, const Ogre::Vector3& target_position);

        Order* order_patrol(const Id& sender_id, const Id& receiver_id, const Id& target_id);
        Order* order_patrol(const Id& sender_id, const Id& receiver_id, const Ogre::Vector3& target_position);

        Order* order_rally(const Id& sender_id, const Id& receiver_id, const Id& target_id);
        Order* order_rally(const Id& sender_id, const Id& receiver_id, const Ogre::Vector3& target_position);

        Order* order_stop(const Id& sender_id, const Id& receiver_id);

        Order* order_unload(const Id& sender_id, const Id& receiver_id, const Id& target_id, const std::string& resource_name, float resource_value);

        Order* order_wait(const Id& sender_id, const Id& receiver_id, float time = Settings::Order::WaitTime);
    };
}
