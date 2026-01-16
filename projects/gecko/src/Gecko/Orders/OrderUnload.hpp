#pragma once

#include "Gecko/Orders/Order.hpp"
#include "Gecko/Resource.hpp"

namespace Gecko
{
    class OrderUnload :
        public Order
    {
    private:
        using base_type = Order;

    public:
        static OrderUnload* create();
        static OrderUnload* create(OrderUnload* memory);
        static OrderUnload* create(OrderUnload* memory, const ConfigurationPtr& configuration);

    public:
        explicit OrderUnload();
        explicit OrderUnload(const OrderUnload& other) = default;

        void init() override;

        ConfigurationPtr serialize() const override;
        void deserialize(const ConfigurationPtr& configuration) override;

    public:
        auto get_target_id() const
        {
            return m_target_id;
        }

        const auto& get_resource_name() const
        {
            return m_resource_name;
        }

        auto get_resource_value() const
        {
            return m_resource_value;
        }

    public:
        void set_target_id(Id target_id)
        {
            m_target_id = target_id;
        }

        void set_resource_name(const std::string& resource_name)
        {
            m_resource_name = resource_name;
        }

        void set_resource_value(float resource_value)
        {
            m_resource_value = resource_value;
        }

    private:
        Id m_target_id;
        std::string m_resource_name;
        float m_resource_value = 0.0f;
    };
}
