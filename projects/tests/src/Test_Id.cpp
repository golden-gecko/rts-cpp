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
    Gecko::Id id(24);

    EXPECT_EQ(id.get(), 24);
    EXPECT_EQ(id.is_valid(), true);
    EXPECT_EQ(id.to_string(), "24");
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

TEST(id, id_pre_incrementation)
{
    Gecko::Id a(2);

    EXPECT_EQ(++a, Gecko::Id(3));
    EXPECT_EQ(a, Gecko::Id(3));
}

TEST(id, id_pre_decrementation)
{
    Gecko::Id a(4);

    EXPECT_EQ(--a, Gecko::Id(3));
    EXPECT_EQ(a, Gecko::Id(3));
}

TEST(id, id_post_incrementation)
{
    Gecko::Id a(6);

    EXPECT_EQ(a++, Gecko::Id(6));
    EXPECT_EQ(a, Gecko::Id(7));
}

TEST(id, id_post_decrementation)
{
    Gecko::Id a(8);

    EXPECT_EQ(a--, Gecko::Id(8));
    EXPECT_EQ(a, Gecko::Id(7));
}
