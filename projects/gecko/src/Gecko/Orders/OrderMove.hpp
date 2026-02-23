#pragma once

#include "Gecko/Navigation/Path.hpp"
#include "Gecko/Orders/Order.hpp"

namespace Gecko
{
    class OrderMove :
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
        static OrderMove* create(OrderMove* memory, const ConfigurationPtr& configuration);

    public:
        explicit OrderMove();
        explicit OrderMove(const OrderMove& other) = default;

    public:
        std::vector<std::shared_ptr<Indicator>> generate_indicators(int order_number) const override;

        std::string get_target_as_string() const override;

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
        void set_path(const Navigation::Path& _path)
        {
            path = _path;
        }

        void set_target_position(const Ogre::Vector3& new_target_position)
        {
            target_position = new_target_position;
        }

    private:
        Ogre::Vector3 target_position = Ogre::Vector3::ZERO;
        Navigation::Path path;
    };
}
