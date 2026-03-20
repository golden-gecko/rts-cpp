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

#include "Fixtures.hpp"

TEST(game, can_init_and_deinit)
{
    std::unique_ptr<Gecko::ConfigurationManager> configuration_manager;
    std::unique_ptr<Gecko::Game> game;

    std::string configuration_name = "rts";
    std::string map_name = "tutorial";

    // Parse configuration files.
    configuration_manager = std::make_unique<Gecko::ConfigurationManager>(false);
    configuration_manager->parse_configuration_files(Gecko::Settings::Configuration::Directories);
    configuration_manager->save_cache();

    // Get configuration.
    Gecko::ConfigurationPtr game_configuration = configuration_manager->get(configuration_name);
    Gecko::ConfigurationPtr map_configuration = configuration_manager->get(map_name);

    // Initialize game.
    game = std::make_unique<Gecko::Game>(game_configuration);
    game->init();

    // Run test.
    EXPECT_NO_THROW(game->init());
    EXPECT_NO_THROW(game->deinit());

    EXPECT_NO_THROW(game->init());
    EXPECT_NO_THROW(game->deinit());
}
