#include "Fixtures.hpp"

#include "Gecko/Games/Game.hpp"
#include "Gecko/Managers/ComponentManager.hpp"
#include "Gecko/Managers/ConfigurationManager.hpp"
#include "Gecko/Managers/MapManager.hpp"
#include "Gecko/Managers/ObjectManager.hpp"
#include "Gecko/Managers/OrderManager.hpp"
#include "Gecko/Managers/PlayerManager.hpp"
#include "Gecko/Managers/SkillManager.hpp"
#include "Gecko/Statistics.hpp"
#include "Gecko/UI/UI.hpp"

init_game::init_game(init_game_config config)
{
    configuration_manager = std::make_shared<Gecko::ConfigurationManager>();
    configuration_manager->parse_configuration_files();

    auto configuration = configuration_manager->get("rts");

    if (config.component_manager)
    {
        component_manager = std::make_shared<Gecko::ComponentManager>();
    }

    if (config.map_manager)
    {
        map_manager = std::make_shared<Gecko::MapManager>();
    }

    if (config.object_manager)
    {
        object_manager = std::make_shared<Gecko::ObjectManager>();
    }

    if (config.order_manager)
    {
        order_manager = std::make_shared<Gecko::OrderManager>();
    }

    if (config.player_manager)
    {
        player_manager = std::make_shared<Gecko::PlayerManager>();
    }

    if (config.skill_manager)
    {
        skill_manager = std::make_shared<Gecko::SkillManager>();
    }

    if (config.statistics)
    {
        statistics = std::make_shared<Gecko::Statistics>();
    }

    if (config.ui)
    {
        ui = std::make_shared<Gecko::UI>(configuration);
    }

    game = std::make_shared<Gecko::Game>(configuration);
    game->init();
}

init_game::~init_game()
{
    component_manager.reset();
    configuration_manager.reset();
    game.reset();
    map_manager.reset();
    object_manager.reset();
    order_manager.reset();
    player_manager.reset();
    skill_manager.reset();
    statistics.reset();
    ui.reset();
}
