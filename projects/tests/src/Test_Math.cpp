#include <gtest/gtest.h>

#include <Gecko/Utils/Math.hpp>

TEST(math, number_is_not_power_of_two)
{
    using namespace Gecko::Utils::Math;

    EXPECT_EQ(is_power_2(0), false);
    EXPECT_EQ(is_power_2(3), false);
    EXPECT_EQ(is_power_2(5), false);
    EXPECT_EQ(is_power_2(6), false);
    EXPECT_EQ(is_power_2(7), false);
    EXPECT_EQ(is_power_2(9), false);
}

TEST(math, number_is_power_of_two)
{
    using namespace Gecko::Utils::Math;

    EXPECT_EQ(is_power_2(1), true);
    EXPECT_EQ(is_power_2(2), true);
    EXPECT_EQ(is_power_2(4), true);
    EXPECT_EQ(is_power_2(8), true);
}
