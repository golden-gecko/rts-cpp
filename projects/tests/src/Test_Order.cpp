#include <gtest/gtest.h>

#include "Orders/Order.hpp"

TEST(order, new_order_is_empty)
{
    auto order = std::make_shared<Gecko::Order>();

    EXPECT_EQ(order->get_configuration().get(), nullptr);
    EXPECT_EQ(order->get_id(), 0);
    EXPECT_EQ(order->get_type(), Gecko::order_type::Value::None);
}
