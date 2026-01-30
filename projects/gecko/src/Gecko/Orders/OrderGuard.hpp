#pragma once

#include "Gecko/Navigation/Path.hpp"
#include "Gecko/Orders/Order.hpp"

namespace Gecko
{
    class OrderGuard :
        public Order
    {
    private:
        using base_type = Order;

    public:
        static OrderGuard* create();
        static OrderGuard* create(OrderGuard* memory);
        static OrderGuard* create(OrderGuard* memory, const ConfigurationPtr& configuration);

    public:
        explicit OrderGuard();
        explicit OrderGuard(const OrderGuard& other) = default;

        void init() override;

        ConfigurationPtr serialize() const override;
        void deserialize(const ConfigurationPtr& configuration) override;

    public:
        const auto& get_path() const
        {
            return path;
        }

        auto get_target_id() const
        {
            return target_id;
        }

        const auto& get_target_position() const
        {
            return target_position;
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

        void set_target_position(const Ogre::Vector3& new_target_position)
        {
            target_position = new_target_position;
        }

    private:
        Id target_id;
        Ogre::Vector3 target_position = Ogre::Vector3::ZERO;
        Navigation::Path path;
    };
}
