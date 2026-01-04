#include <gtest/gtest.h>

#include <Gecko/Configuration.hpp>
#include <Gecko/Containers/Selected.hpp>
#include <Gecko/Players/Player.hpp>

TEST(player, new_player_is_empty)
{
    auto player = std::make_shared<Gecko::Player>();

    EXPECT_EQ(player->get_configuration().get(), nullptr);
    EXPECT_EQ(player->get_id(), 0);
    EXPECT_EQ(player->get_color(), "");
    EXPECT_EQ(player->get_name(), "");
    EXPECT_EQ(player->get_selected()->size(), 0);
}

TEST(player, player_deserialized_from_configuration_is_loaded)
{
    auto player_configuration = std::make_shared<Gecko::Configuration>("../players/human.json");
    auto player = std::make_shared<Gecko::Player>();

    player->deserialize(player_configuration);

    EXPECT_EQ(player->get_configuration()->get_name(), "human");
    EXPECT_EQ(player->get_id(), 0);
    EXPECT_EQ(player->get_color(), "blue");
    EXPECT_EQ(player->get_name(), "Human");
    EXPECT_EQ(player->get_selected()->size(), 0);
}

TEST(player, player_deserialized_and_initialized_from_configuration_is_loaded)
{
    auto player_configuration = std::make_shared<Gecko::Configuration>("../players/human.json");
    auto player = std::make_shared<Gecko::Player>();

    player->deserialize(player_configuration);
    player->init();

    EXPECT_EQ(player->get_configuration()->get_name(), "human");
    EXPECT_EQ(player->get_id(), 0);
    EXPECT_EQ(player->get_color(), "blue");
    EXPECT_EQ(player->get_name(), "Human");
    EXPECT_EQ(player->get_selected()->size(), 0);
}
