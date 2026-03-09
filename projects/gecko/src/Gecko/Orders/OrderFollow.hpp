#pragma once

#include "Gecko/Navigation/Path.hpp"
#include "Gecko/Orders/Order.hpp"

namespace Gecko
{
    class OrderFollow :
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
        static OrderFollow* create(OrderFollow* memory, const ConfigurationPtr& configuration);

    public:
        explicit OrderFollow();
        OrderFollow(const OrderFollow& other) = default;

    public:
        const auto& get_path() const
        {
            return path;
        }

        auto get_target_id() const
        {
            return target_id;
        }

    public:
        void set_path(const Navigation::Path& _path)
        {
            path = _path;
        }

        void set_target_id(const Id& _target_id)
        {
            target_id = _target_id;
        }

    private:
        Id target_id;
        Navigation::Path path;
    };
}
