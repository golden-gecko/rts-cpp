#include <gtest/gtest.h>

#include "Timer.hpp"

TEST(timer, new_timer_is_empty)
{
    Gecko::Timer timer;

    EXPECT_EQ(timer.update(0.0f), false);
    EXPECT_EQ(timer.update(2.0f), false);
}

TEST(timer, new_timer_with_maximal_value_set_is_not_full)
{
    Gecko::Timer timer(10.0);

    EXPECT_EQ(timer.update(0.0f), false);
    EXPECT_EQ(timer.update(2.0f), false);
}

TEST(timer, new_timer_with_maximal_value_set_is_full_after_updates)
{
    Gecko::Timer timer(40.0);

    EXPECT_EQ(timer.update(40.0f), true);
    EXPECT_EQ(timer.update(80.0f), true);
}

TEST(timer, full_timer_is_empty_after_reset)
{
    Gecko::Timer timer(30.0);

    EXPECT_EQ(timer.update(30.0f), true);

    timer.reset();

    EXPECT_EQ(timer.update(10.0f), false);
}
