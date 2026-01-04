#include "Fixtures.hpp"

init_game::init_game()
{
    configuration_manager = std::make_shared<Gecko::ConfigurationManager>();
    configuration_manager->parse_configuration_files();

    component_manager = std::make_shared<Gecko::ComponentManager>();
    map_manager = std::make_shared<Gecko::MapManager>();
    object_manager = std::make_shared<Gecko::ObjectManager>();
    order_manager = std::make_shared<Gecko::OrderManager>();
    player_manager = std::make_shared<Gecko::PlayerManager>();
    skill_manager = std::make_shared<Gecko::SkillManager>();
    statistics = std::make_shared<Gecko::Statistics>();
    ui = std::make_shared<Gecko::UI>(configuration_manager->get("rts"));

    game = std::make_shared<Gecko::Game>(configuration_manager->get("rts"));
    game->init();
}

init_game::~init_game()
{
    game->deinit();
}
