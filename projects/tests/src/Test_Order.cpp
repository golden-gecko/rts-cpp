#include <gtest/gtest.h>

#include <Gecko/Orders/Order.hpp>

TEST(order, new_order_is_empty)
{
    Gecko::Order order;

    EXPECT_EQ(order.get_configuration().get(), nullptr);
    EXPECT_EQ(order.get_id(), Gecko::Id::Empty);
    EXPECT_EQ(order.get_type(), Gecko::order_type::Value::None);
}
