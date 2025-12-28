#pragma once

#include "Gecko/Orders/Order.hpp"
#include "Gecko/Path.hpp"

namespace Gecko
{
    class OrderMove : public Order
    {
    private:
        using base_type = Order;

    public:
        static OrderMove* create();
        static OrderMove* create(OrderMove* memory);
        static OrderMove* create(OrderMove* memory, const std::shared_ptr<Configuration>& configuration);

    public:
        explicit OrderMove();
        explicit OrderMove(const OrderMove& other) = default;

        void init() override;

        std::shared_ptr<Configuration> serialize() const override;
        void deserialize(const std::shared_ptr<Configuration>& configuration) override;

    public:
        // TODO: Remove.
        auto& get_path()
        {
            return path;
        }

        const auto& get_path() const
        {
            return path;
        }

        // TODO: Remove.
        auto& get_points()
        {
            return path.get_points();
        }

        const auto& get_points() const
        {
            return path.get_points();
        }

        const auto& get_target_position() const
        {
            return target_position;
        }

    public:
        void set_path(const Path& _path)
        {
            path = _path;
        }

        void set_target_position(const Ogre::Vector3& new_target_position)
        {
            target_position = new_target_position;
        }

    private:
        Ogre::Vector3 target_position = Ogre::Vector3::ZERO;
        Path path;
    };
}
