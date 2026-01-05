#pragma once

#include "Gecko/Orders/Order.hpp"

namespace Gecko
{
    class OrderRally :
        public Order
    {
    private:
        using base_type = Order;

    public:
        static OrderRally* create();
        static OrderRally* create(OrderRally* memory);
        static OrderRally* create(OrderRally* memory, const std::shared_ptr<Configuration>& configuration);

    public:
        explicit OrderRally();
        explicit OrderRally(const OrderRally& other) = default;

        void init() override;

        std::shared_ptr<Configuration> serialize() const override;
        void deserialize(const std::shared_ptr<Configuration>& configuration) override;

    public:
        const auto& get_target_position() const
        {
            return target_position;
        }

    public:
        void set_target_position(const Ogre::Vector3& new_target_position)
        {
            target_position = new_target_position;
        }

    private:
        Id target_id;
        Ogre::Vector3 target_position = Ogre::Vector3::ZERO;
    };
}
