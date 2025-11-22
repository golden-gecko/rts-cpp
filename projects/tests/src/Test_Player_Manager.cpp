#include <gtest/gtest.h>

#include "Configuration.hpp"
#include "Managers/PlayerManager.hpp"
#include "Players/Player.hpp"

TEST(player_manager, new_player_manager_is_empty)
{
    auto player_manager = std::make_shared<Gecko::PlayerManager>();

    EXPECT_EQ(player_manager->is_type_registered("player"), false);
}

TEST(player_manager, new_player_manager_can_register_types)
{
    auto player_manager = std::make_shared<Gecko::PlayerManager>();
    auto player_function = static_cast<Gecko::Player*(*)(Gecko::Player* memory, const std::shared_ptr<Gecko::Configuration>&)>(&Gecko::Player::create);
    auto player_factory = std::bind(player_function, std::placeholders::_1, std::make_shared<Gecko::Configuration>());

    player_manager->register_type<Gecko::Player>("player", player_factory);

    EXPECT_EQ(player_manager->get_size("player"), 0);
    EXPECT_EQ(player_manager->get_max_size("player"), 0);
}

TEST(player_manager, new_player_manager_can_allocate_memory)
{
    auto player_manager = std::make_shared<Gecko::PlayerManager>();
    auto player_configuration = std::make_shared<Gecko::Configuration>("../players/human.json");
    auto player_function = static_cast<Gecko::Player*(*)(Gecko::Player* memory, const std::shared_ptr<Gecko::Configuration>&)>(&Gecko::Player::create);
    auto player_factory = std::bind(player_function, std::placeholders::_1, player_configuration);

    player_manager->register_type<Gecko::Player>("player", player_factory);
    player_manager->allocate("player", 2);

    EXPECT_EQ(player_manager->get_size("player"), 0);
    EXPECT_EQ(player_manager->get_max_size("player"), 2);
}
