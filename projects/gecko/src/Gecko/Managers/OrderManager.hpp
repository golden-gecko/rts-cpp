#pragma once

#include "Gecko/Id.hpp"
#include "Gecko/Interfaces/Initializable.hpp"
#include "Gecko/Interfaces/Updatable.hpp"
#include "Gecko/Managers/Manager.hpp"
#include "Gecko/Orders/Order.hpp"
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
        // From Initializable.
        void init();
        void deinit();

    public:
        // From Updatable.
        void update(float time) override;

    public:
        OrderManager(const ConfigurationPtr& configuration);

    public:
        OrderPtr order_attack(const Id& sender_id, const Id& receiver_id, const Id& target_id);
        OrderPtr order_attack(const Id& sender_id, const Id& receiver_id, const Ogre::Vector3& target_position);

        OrderPtr order_create(const Id& sender_id, const Id& receiver_id, const std::string& configuration_name, const Ogre::Vector3& position, const Id& player_id);

        OrderPtr order_destroy(const Id& sender_id, const Id& receiver_id);

        OrderPtr order_disable(const Id& sender_id, const Id& receiver_id);

        OrderPtr order_enable(const Id& sender_id, const Id& receiver_id);

        OrderPtr order_follow(const Id& sender_id, const Id& receiver_id, const Id& target_id);

        OrderPtr order_guard(const Id& sender_id, const Id& receiver_id, const Id& target_id);
        OrderPtr order_guard(const Id& sender_id, const Id& receiver_id, const Ogre::Vector3& target_position);

        OrderPtr order_load(const Id& sender_id, const Id& receiver_id, const Id& target_id, const std::string& resource_name, float resource_value);

        OrderPtr order_move(const Id& sender_id, const Id& receiver_id, const Id& target_id);
        OrderPtr order_move(const Id& sender_id, const Id& receiver_id, const Ogre::Vector3& target_position);

        OrderPtr order_patrol(const Id& sender_id, const Id& receiver_id, const Id& target_id);
        OrderPtr order_patrol(const Id& sender_id, const Id& receiver_id, const Ogre::Vector3& target_position);

        OrderPtr order_rally(const Id& sender_id, const Id& receiver_id, const Id& target_id);
        OrderPtr order_rally(const Id& sender_id, const Id& receiver_id, const Ogre::Vector3& target_position);

        OrderPtr order_stop(const Id& sender_id, const Id& receiver_id);

        OrderPtr order_unload(const Id& sender_id, const Id& receiver_id, const Id& target_id, const std::string& resource_name, float resource_value);

        OrderPtr order_wait(const Id& sender_id, const Id& receiver_id, float time = Settings::Order::WaitTime);

    private:
        ConfigurationPtr m_configuration;
    };
}
