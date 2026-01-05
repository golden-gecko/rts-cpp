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
        static OrderUnload* create(OrderUnload* memory, const std::shared_ptr<Configuration>& configuration);

    public:
        explicit OrderUnload();
        explicit OrderUnload(const OrderUnload& other) = default;

        void init() override;

        std::shared_ptr<Configuration> serialize() const override;
        void deserialize(const std::shared_ptr<Configuration>& configuration) override;

    public:
        auto get_target_id() const
        {
            return target_id;
        }

        const auto& get_resource_name() const
        {
            return resource_name;
        }

        auto get_resource_value() const
        {
            return resource_value;
        }

    public:
        void set_target_id(Id new_target_id)
        {
            target_id = new_target_id;
        }

        void set_resource_name(const std::string& new_resource_name)
        {
            resource_name = new_resource_name;
        }

        void set_resource_value(float new_resource_value)
        {
            resource_value = new_resource_value;
        }

    private:
        Id target_id;
        std::string resource_name;
        float resource_value = 0.0f;
    };
}
