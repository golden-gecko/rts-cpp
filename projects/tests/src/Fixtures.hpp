#include <Gecko/Games/Game.hpp>
#include <Gecko/Managers/ComponentManager.hpp>
#include <Gecko/Managers/ConfigurationManager.hpp>
#include <Gecko/Managers/MapManager.hpp>
#include <Gecko/Managers/ObjectManager.hpp>
#include <Gecko/Managers/OrderManager.hpp>
#include <Gecko/Managers/PlayerManager.hpp>
#include <Gecko/Managers/SkillManager.hpp>
#include <Gecko/Statistics.hpp>
#include <Gecko/UI/UI.hpp>

struct init_game
{
    std::shared_ptr<Gecko::ComponentManager> component_manager;
    std::shared_ptr<Gecko::ConfigurationManager> configuration_manager;
    std::shared_ptr<Gecko::Game> game;
    std::shared_ptr<Gecko::MapManager> map_manager;
    std::shared_ptr<Gecko::ObjectManager> object_manager;
    std::shared_ptr<Gecko::OrderManager> order_manager;
    std::shared_ptr<Gecko::PlayerManager> player_manager;
    std::shared_ptr<Gecko::SkillManager> skill_manager;
    std::shared_ptr<Gecko::Statistics> statistics;

    std::shared_ptr<Gecko::UI> ui;

    init_game();

    ~init_game();
};
