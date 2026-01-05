#pragma once

#include "Gecko/Orders/Order.hpp"

namespace Gecko
{
    class OrderAttack :
        public Order
    {
    private:
        using base_type = Order;

    public:
        static OrderAttack* create();
        static OrderAttack* create(OrderAttack* memory);
        static OrderAttack* create(OrderAttack* memory, const std::shared_ptr<Configuration>& configuration);

    public:
        explicit OrderAttack();
        explicit OrderAttack(const OrderAttack& other) = default;

        void init() override;

        std::shared_ptr<Configuration> serialize() const override;
        void deserialize(const std::shared_ptr<Configuration>& configuration) override;

    public:
        auto get_target_id() const
        {
            return target_id;
        }

        const auto& get_target_position() const
        {
            return target_position;
        }

    public:
        void set_target_id(const Id& new_target_id)
        {
            target_id = new_target_id;
        }

        void set_target_position(const Ogre::Vector3& new_target_position)
        {
            target_position = new_target_position;
        }

    private:
        Id target_id;
        Ogre::Vector3 target_position = Ogre::Vector3::ZERO;
    };
}
