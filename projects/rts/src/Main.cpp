#include <Gecko/Application.hpp>
#include <Gecko/Configuration.hpp>
#include <Gecko/Games/Game.hpp>
#include <Gecko/Input.hpp>
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
#include <Gecko/Settings.hpp>
#include <Gecko/Statistics.hpp>
#include <Gecko/Technologies/TechnologyTree.hpp>
#include <Gecko/UI/UI.hpp>
#include <Gecko/Utils/Utils.hpp>

int main(int argc, char* argv[])
{
    // Setup log.
    Gecko::Log::setup("rts.log");

    // Parse command line options.
    auto options = Gecko::Application::parse_options(argc, argv, "rts", "tutorial");

    auto configuration_name = Gecko::Application::get_option(options, "configuration");
    auto map_name = Gecko::Application::get_option(options, "map");

    // Parse configuration files.
    auto configuration_manager = std::make_unique<Gecko::ConfigurationManager>(false);

    configuration_manager->parse_configuration_files(Gecko::Settings::Configuration::Directories);
    configuration_manager->save_cache();

    // Get configuration.
    auto configuration = configuration_manager->get(configuration_name);

    // Create global objects.
    auto game = std::make_unique<Gecko::Game>(configuration);
    auto input = std::make_unique<Gecko::Input>(configuration->get_child("input"));
    auto statistics = std::make_unique<Gecko::Statistics>();
    auto technology_tree = std::make_unique<Gecko::TechnologyTree>();
    auto ui = std::make_unique<Gecko::UI>(configuration->get_child("ui"));

    // Initialize game.
    game->init();

    // Create and initialize managers.
    auto component_manager = std::make_unique<Gecko::ComponentManager>();
    auto job_manager = std::make_unique<Gecko::JobManager>();
    auto map_manager = std::make_unique<Gecko::MapManager>();
    auto object_manager = std::make_unique<Gecko::ObjectManager>();
    auto order_manager = std::make_unique<Gecko::OrderManager>();
    auto player_manager = std::make_unique<Gecko::PlayerManager>();
    auto skill_manager = std::make_unique<Gecko::SkillManager>();

    skill_manager->init(configuration);
    component_manager->init(configuration);
    order_manager->init(configuration);
    player_manager->init(configuration);
    object_manager->init(configuration);
    map_manager->init(configuration);

    // Load map.
    game->load_map(map_name);

    // Initialize input system.
    input->set_render_window_handle(Gecko::Utils::get_window_handle(game->getRenderWindow()));
    input->init();

    // Initialize other system.
    technology_tree->init();
    ui->init();

    // Run game.
    game->load_options();
    game->run();

    // Deinit in correct order.
    ui->deinit();
    technology_tree->deinit();
    input->deinit();

    /*
    skill_manager->deinit();
    component_manager->deinit();
    order_manager->deinit();
    player_manager->deinit();
    object_manager->deinit();
    map_manager->deinit();
    */

    game->deinit();

    return 0;
}
