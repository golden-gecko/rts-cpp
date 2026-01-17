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
#include <Gecko/Statistics.hpp>
#include <Gecko/Technologies/TechnologyTree.hpp>
#include <Gecko/UI/UI.hpp>
#include <Gecko/Utils/Utils.hpp>

int main(int argc, char* argv[])
{
    // Setup log.
    Gecko::Log::setup("rts.log");

    // Parse configuration files.
    auto configuration_manager = std::make_unique<Gecko::ConfigurationManager>(false);

    configuration_manager->parse_configuration_files();
    configuration_manager->save_cache();

    // Parse command line options.
    auto options = Gecko::Application::parse_options(argc, argv, "rts");
    const auto& configuration_name = options["configuration"].as<std::string>();
    auto configuration = configuration_manager->get(configuration_name);

    // Create global objects.
    auto component_manager = std::make_unique<Gecko::ComponentManager>();
    auto game = std::make_unique<Gecko::Game>(configuration);
    auto input = std::make_unique<Gecko::Input>(configuration->get_child("input"));
    auto job_manager = std::make_unique<Gecko::JobManager>();
    auto map_manager = std::make_unique<Gecko::MapManager>();
    auto object_manager = std::make_unique<Gecko::ObjectManager>();
    auto order_manager = std::make_unique<Gecko::OrderManager>();
    auto player_manager = std::make_unique<Gecko::PlayerManager>();
    auto skill_manager = std::make_unique<Gecko::SkillManager>();
    auto statistics = std::make_unique<Gecko::Statistics>();
    auto technology_tree = std::make_unique<Gecko::TechnologyTree>();
    auto ui = std::make_unique<Gecko::UI>(configuration->get_child("ui"));

    // Initialize game and load map.
    game->init();
    game->load_map("tutorial");

    // Initialize input system.
    input->set_render_window_handle(Gecko::Utils::get_window_handle(game->getRenderWindow()));
    input->init();

    // Initialize other system.
    technology_tree->init();
    ui->init();

    // Run game.
    game->load_options();
    game->run();

    // Deinitialize other system.
    input->deinit();
    technology_tree->deinit();
    ui->deinit();

    // Denitialize game.
    game->deinit();

    return 0;
}

/*

    "objects": [
        {
            "configuration": "ironworks",
            "player": "human",
            "position": {
                "x": 160.0,
                "z": 128.0
            }
        },
        {
            "configuration": "tools_factory",
            "player": "human",
            "position": {
                "x": 168.0,
                "z": 128.0
            }
        },
        {
            "configuration": "vehicles_factory",
            "player": "human",
            "position": {
                "x": 176.0,
                "z": 128.0
            }
        }
    ],

*/