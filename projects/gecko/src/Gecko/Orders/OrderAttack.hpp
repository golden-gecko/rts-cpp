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
        // From Initializable.
        void init() override;

    public:
        // From Serializable.
        ConfigurationPtr serialize() const override;
        void deserialize(const ConfigurationPtr& configuration) override;

    public:
        static OrderAttack* create(OrderAttack* memory, const ConfigurationPtr& configuration);

    public:
        explicit OrderAttack();
        explicit OrderAttack(const OrderAttack& other) = default;

    public:
        auto get_target_id() const
        {
            return m_target_id;
        }

        const auto& get_target_position() const
        {
            return m_target_position;
        }

    public:
        void set_target_id(const Id& target_id)
        {
            m_target_id = target_id;
        }

        void set_target_position(const Ogre::Vector3& target_position)
        {
            m_target_position = target_position;
        }

    private:
        Id            m_target_id;
        Ogre::Vector3 m_target_position = Ogre::Vector3::ZERO;
    };
}
