#include <Gecko/Application.hpp>
#include <Gecko/Configuration.hpp>
#include <Gecko/Games/Game.hpp>
#include <Gecko/Input/Input.hpp>
#include <Gecko/Log.hpp>
#include <Gecko/Managers/ComponentManager.hpp>
#include <Gecko/Managers/ConfigurationManager.hpp>
#include <Gecko/Managers/JobManager.hpp>
#include <Gecko/Managers/MapManager.hpp>
#include <Gecko/Managers/ObjectManager.hpp>
#include <Gecko/Managers/OrderManager.hpp>
#include <Gecko/Managers/PlayerManager.hpp>
#include <Gecko/Managers/SkillManager.hpp>
#include <Gecko/NVIDIA.hpp>
#include <Gecko/Players/Player.hpp>
#include <Gecko/Scenes/Scene.hpp>
#include <Gecko/Settings.hpp>
#include <Gecko/Statistics.hpp>
#include <Gecko/Technologies/TechnologyTree.hpp>
#include <Gecko/UI/RenderInterface.hpp>
#include <Gecko/UI/UI.hpp>
#include <Gecko/Utils/Utils.hpp>

int main(int argc, char* argv[])
{
    std::unique_ptr<Gecko::ComponentManager> component_manager;
    std::unique_ptr<Gecko::ConfigurationManager> configuration_manager;
    std::unique_ptr<Gecko::Game> game;
    std::unique_ptr<Gecko::Input> input;
    std::unique_ptr<Gecko::JobManager> job_manager;
    std::unique_ptr<Gecko::MapManager> map_manager;
    std::unique_ptr<Gecko::ObjectManager> object_manager;
    std::unique_ptr<Gecko::OrderManager> order_manager;
    std::unique_ptr<Gecko::PlayerManager> player_manager;
    std::unique_ptr<Gecko::SkillManager> skill_manager;
    std::unique_ptr<Gecko::Statistics> statistics;
    std::unique_ptr<Gecko::TechnologyTree> technology_tree;
    std::unique_ptr<Gecko::UI> ui;
    
    // Setup log.
    Gecko::Log::setup("rts.log");

    // Parse command line options.
    auto options = Gecko::Application::parse_options(argc, argv, "rts", "tutorial");
    auto configuration_name = Gecko::Application::get_option(options, "configuration");
    auto map_name = Gecko::Application::get_option(options, "map");

    // Parse configuration files.
    configuration_manager = std::make_unique<Gecko::ConfigurationManager>(false);
    configuration_manager->parse_configuration_files(Gecko::Settings::Configuration::Directories);
    configuration_manager->save_cache();

    // Get configuration.
    auto game_configuration = configuration_manager->get(configuration_name);
    auto map_configuration = configuration_manager->get(map_name);

    // Initialize game.
    game = std::make_unique<Gecko::Game>(game_configuration);
    game->init();

    // Initialize input system.
    input = std::make_unique<Gecko::Input>(game_configuration->get_child("input"), Gecko::Utils::get_window_handle(game->getRenderWindow()));
    input->init();

    // Initialize managers.
    job_manager = std::make_unique<Gecko::JobManager>();

    skill_manager = std::make_unique<Gecko::SkillManager>();
    skill_manager->init(game_configuration);

    component_manager = std::make_unique<Gecko::ComponentManager>();
    component_manager->init(game_configuration, game->get_map_scene());

    order_manager = std::make_unique<Gecko::OrderManager>();
    order_manager->init(game_configuration);

    player_manager = std::make_unique<Gecko::PlayerManager>();
    player_manager->init(game_configuration);

    object_manager = std::make_unique<Gecko::ObjectManager>();
    object_manager->init(game_configuration, game->get_map_scene());

    map_manager = std::make_unique<Gecko::MapManager>();
    map_manager->init(game_configuration, game->get_map_scene());

    // Load map.
    game->load_map(map_name);

    // Initialize technology tree.
    technology_tree = std::make_unique<Gecko::TechnologyTree>();
    technology_tree->init();

    // Initialize statistics.
    statistics = std::make_unique<Gecko::Statistics>();

    // Initialize UI.
    ui = std::make_unique<Gecko::UI>(game_configuration->get_child("ui"), game->get_map_scene());
    ui->init();

    // Set render queue listener for rendering UI.
    game->get_map_scene()->get_scene_manager()->addRenderQueueListener(ui->get_render_interface().get());

    // Set active player.
    if (Gecko::PlayerPtr player = Gecko::PlayerManager::getSingleton().get_by_configuration_name(map_configuration->get_string("active_player")))
    {
        game->set_active_player_id(player->get_id());
    }

    // Run game.
    game->load_options();
    game->run();

    return 0;
}
