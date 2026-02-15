#pragma once

#include "Gecko/Orders/Order.hpp"

namespace Gecko
{
    class OrderCreate :
        public Order
    {
    private:
        using base_type = Order;

    public:
        static OrderCreate* create(OrderCreate* memory, const ConfigurationPtr& configuration);

    public:
        explicit OrderCreate();
        explicit OrderCreate(const OrderCreate& other) = default;

        void init() override;

        ConfigurationPtr serialize() const override;
        void deserialize(const ConfigurationPtr& configuration) override;

    public:
        auto get_player_id() const
        {
            return player_id;
        }

        const auto& get_configuration_name() const
        {
            return configuration_name;
        }

        const auto& get_position() const
        {
            return position;
        }

    public:
        void set_player_id(const Id& new_player_id)
        {
            player_id = new_player_id;
        }

        void set_configuration_name(const std::string& new_configuration_name)
        {
            configuration_name = new_configuration_name;
        }

        void set_position(const Ogre::Vector3& new_position)
        {
            position = new_position;
        }

    private:
        Id player_id;
        std::string configuration_name;
        Ogre::Vector3 position;
    };
}
