#include <gtest/gtest.h>

#include <Gecko/Configuration.hpp>
#include <Gecko/Item.hpp>

namespace
{
    class Character :
        public Gecko::Item
    {
    public:
        void update(float time) override
        {
        }
    };
}

TEST(item, new_item_is_empty)
{
    auto item = std::make_shared<Character>();

    EXPECT_EQ(item->get_configuration().get(), nullptr);
    EXPECT_EQ(item->get_id().is_valid(), false);
}

TEST(item, deserialized_item_is_not_initialized)
{
    auto configuration = std::make_shared<Gecko::Configuration>("../objects/missiles/bullet.json");
    auto item = std::make_shared<Character>();

    item->deserialize(configuration);

    EXPECT_EQ(item->get_configuration()->get_name(), "bullet");
    EXPECT_EQ(item->get_id().is_valid(), false);
}

TEST(item, item_initialization)
{
    auto configuration = std::make_shared<Gecko::Configuration>("../objects/vehicles/military/tank.json");
    auto item = std::make_shared<Character>();

    item->deserialize(configuration);
    item->init();

    EXPECT_EQ(item->get_configuration()->get_name(), "tank");
}

TEST(item, item_is_deserialized_after_deinitialization)
{
    auto configuration = std::make_shared<Gecko::Configuration>("../objects/plants/tree_01.json");
    auto item = std::make_shared<Character>();

    item->deserialize(configuration);
    item->init();
    item->deinit();

    EXPECT_EQ(item->get_configuration()->get_name(), "tree_01");
    EXPECT_EQ(item->get_id().is_valid(), false);
}
