#include <gtest/gtest.h>

#include <Gecko/Configuration.hpp>
#include <Gecko/Games/Game.hpp>
#include <Gecko/Managers/ComponentManager.hpp>
#include <Gecko/Managers/ConfigurationManager.hpp>
#include <Gecko/Managers/MapManager.hpp>
#include <Gecko/Managers/ObjectManager.hpp>
#include <Gecko/Managers/OrderManager.hpp>
#include <Gecko/Managers/PlayerManager.hpp>
#include <Gecko/Managers/SkillManager.hpp>

TEST(game, can_init_and_deinit)
{
    auto component_manager = std::make_shared<Gecko::ComponentManager>();
    auto configuration_manager = std::make_shared<Gecko::ConfigurationManager>();
    auto map_manager = std::make_shared<Gecko::MapManager>();
    auto object_manager = std::make_shared<Gecko::ObjectManager>();
    auto order_manager = std::make_shared<Gecko::OrderManager>();
    auto player_manager = std::make_shared<Gecko::PlayerManager>();
    auto skill_manager = std::make_shared<Gecko::SkillManager>();

    auto game = std::make_shared<Gecko::Game>(std::make_shared<Gecko::Configuration>("../games/rts.json"));

    EXPECT_NO_THROW(game->init());
    EXPECT_NO_THROW(game->deinit());

    EXPECT_NO_THROW(game->init());
    EXPECT_NO_THROW(game->deinit());
}
