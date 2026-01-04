#include <gtest/gtest.h>

#include <Gecko/Id.hpp>

TEST(id, new_id_is_empty_and_invalid)
{
    Gecko::Id id;

    EXPECT_EQ(id.get(), 0);
    EXPECT_EQ(id.is_valid(), false);
    EXPECT_EQ(id.to_string(), "0");
}

TEST(id, initialized_id_is_valid)
{
    Gecko::Id id(10);

    EXPECT_EQ(id.get(), 10);
    EXPECT_EQ(id.is_valid(), true);
    EXPECT_EQ(id.to_string(), "10");
}

TEST(id, id_is_lower)
{
    Gecko::Id a(2);
    Gecko::Id b(4);

    EXPECT_LT(a, b);
}

TEST(id, id_is_greater)
{
    Gecko::Id a(8);
    Gecko::Id b(6);

    EXPECT_GT(a, b);
}

TEST(id, id_incrementation)
{
    Gecko::Id a(2);

    EXPECT_EQ(++a, 3);
}

TEST(id, id_decrementation)
{
    Gecko::Id a(8);

    EXPECT_EQ(--a, 7);
}
