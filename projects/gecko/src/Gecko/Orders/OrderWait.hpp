#pragma once

#include "Gecko/Orders/Order.hpp"
#include "Gecko/Timer.hpp"

namespace Gecko
{
    class OrderWait :
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
        static OrderWait* create(OrderWait* memory, const ConfigurationPtr& configuration);

    public:
        explicit OrderWait();
        OrderWait(const OrderWait& other) = default;

    public:
        std::string get_target_as_string() const override;

    public:
        Timer& get_wait_timer()
        {
            return m_wait_timer;
        }

        const Timer& get_wait_timer() const
        {
            return m_wait_timer;
        }

    private:
        Timer m_wait_timer;
    };
}
