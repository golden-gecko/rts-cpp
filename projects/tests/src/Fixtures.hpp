#include <Gecko/Diplomacy.hpp>
#include <Gecko/Games/Game.hpp>
#include <Gecko/Managers/ComponentManager.hpp>
#include <Gecko/Managers/ConfigurationManager.hpp>
#include <Gecko/Managers/JobManager.hpp>
#include <Gecko/Managers/MapManager.hpp>
#include <Gecko/Managers/ObjectManager.hpp>
#include <Gecko/Managers/OrderManager.hpp>
#include <Gecko/Managers/PlayerManager.hpp>
#include <Gecko/Managers/SkillManager.hpp>
#include <Gecko/Statistics.hpp>
#include <Gecko/Technologies/TechnologyTree.hpp>
#include <Gecko/UI/UI.hpp>

struct init_game
{
    std::unique_ptr<Gecko::ComponentManager> component_manager;
    std::unique_ptr<Gecko::ConfigurationManager> configuration_manager;
    std::unique_ptr<Gecko::Diplomacy> diplomacy;
    std::unique_ptr<Gecko::Game> game;
    std::unique_ptr<Gecko::JobManager> job_manager;
    std::unique_ptr<Gecko::MapManager> map_manager;
    std::unique_ptr<Gecko::ObjectManager> object_manager;
    std::unique_ptr<Gecko::OrderManager> order_manager;
    std::unique_ptr<Gecko::PlayerManager> player_manager;
    std::unique_ptr<Gecko::SkillManager> skill_manager;
    std::unique_ptr<Gecko::Statistics> statistics;
    std::unique_ptr<Gecko::TechnologyTree> technology_tree;
    std::unique_ptr<Gecko::UI> ui;

    init_game();

    ~init_game();
};
