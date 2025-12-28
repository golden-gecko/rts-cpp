#pragma once

#include "Gecko/Orders/Order.hpp"
#include "Gecko/Path.hpp"

namespace Gecko
{
    class OrderFollow : public Order
    {
    private:
        using base_type = Order;

    public:
        static OrderFollow* create();
        static OrderFollow* create(OrderFollow* memory);
        static OrderFollow* create(OrderFollow* memory, const std::shared_ptr<Configuration>& configuration);

    public:
        explicit OrderFollow();
        explicit OrderFollow(const OrderFollow& other) = default;

        void init() override;

        std::shared_ptr<Configuration> serialize() const override;
        void deserialize(const std::shared_ptr<Configuration>& configuration) override;

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
        void set_path(const Path& _path)
        {
            path = _path;
        }

        void set_target_id(const Id& _target_id)
        {
            target_id = _target_id;
        }

    private:
        Id target_id;
        Path path;
    };
}
