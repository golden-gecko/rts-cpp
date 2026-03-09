#include "Fixtures.hpp"

#include <Gecko/Configuration.hpp>
#include <Gecko/Players/Player.hpp>
#include <Gecko/Scenes/Scene.hpp>
#include <Gecko/Settings.hpp>

init_game::init_game()
{
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

    // Initialize statistics.
    statistics = std::make_unique<Gecko::Statistics>();

    // Initialize managers.
    job_manager = std::make_unique<Gecko::JobManager>();

    skill_manager = std::make_unique<Gecko::SkillManager>(game_configuration);
    skill_manager->init();

    component_manager = std::make_unique<Gecko::ComponentManager>(game_configuration, game->get_map_scene());
    component_manager->init();

    order_manager = std::make_unique<Gecko::OrderManager>(game_configuration);
    order_manager->init();

    player_manager = std::make_unique<Gecko::PlayerManager>(game_configuration);
    player_manager->init();

    object_manager = std::make_unique<Gecko::ObjectManager>(game_configuration, game->get_map_scene());
    object_manager->init();

    map_manager = std::make_unique<Gecko::MapManager>(game_configuration, game->get_map_scene());
    map_manager->init();

    // Initialize diplomacy.
    diplomacy = std::make_unique<Gecko::Diplomacy>();
    diplomacy->deserialize(map_configuration->get_child("diplomacy"));

    // Initialize technology tree.
    technology_tree = std::make_unique<Gecko::TechnologyTree>();
    technology_tree->init();
}

init_game::~init_game()
{
}
