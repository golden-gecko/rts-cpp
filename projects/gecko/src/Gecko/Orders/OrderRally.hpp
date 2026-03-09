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
        // From Initializable.
        void init() override;

    public:
        // From Serializable.
        ConfigurationPtr serialize() const override;
        void deserialize(const ConfigurationPtr& configuration) override;

    public:
        static OrderRally* create(OrderRally* memory, const ConfigurationPtr& configuration);

    public:
        explicit OrderRally();
        OrderRally(const OrderRally& other) = default;

    public:
        const auto& get_target_position() const
        {
            return m_target_position;
        }

    public:
        void set_target_position(const Ogre::Vector3& target_position)
        {
            m_target_position = target_position;
        }

    private:
        Id            m_target_id;
        Ogre::Vector3 m_target_position = Ogre::Vector3::ZERO;
    };
}
