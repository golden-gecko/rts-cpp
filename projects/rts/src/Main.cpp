#include "Gecko/Application.hpp"
#include "Gecko/Games/Game.hpp"
#include "Gecko/Input.hpp"
#include "Gecko/Log.hpp"
#include "Gecko/Managers/ComponentManager.hpp"
#include "Gecko/Managers/ConfigurationManager.hpp"
#include "Gecko/Managers/JobManager.hpp"
#include "Gecko/Managers/MapManager.hpp"
#include "Gecko/Managers/ObjectManager.hpp"
#include "Gecko/Managers/OrderManager.hpp"
#include "Gecko/Managers/PlayerManager.hpp"
#include "Gecko/Managers/SkillManager.hpp"
#include "Gecko/Maps/Map.hpp"
#include "Gecko/NVIDIA.hpp"
#include "Gecko/Statistics.hpp"
#include "Gecko/Technologies/TechnologyTree.hpp"
#include "Gecko/UI/UI.hpp"
#include "Gecko/Window.hpp"

int main(int argc, char* argv[])
{
    Gecko::Log::setup();

    // Parse configuration files.
    auto configuration_manager = std::make_unique<Gecko::ConfigurationManager>();
    configuration_manager->parse_configuration_files();

    // Parse command line options.
    auto options = Gecko::Application::parse_options(argc, argv, "rts");
    const auto& configuration_name = options["configuration"].as<std::string>();
    auto configuration = configuration_manager->get(configuration_name);

    // Create global objects.
    auto component_manager = std::make_unique<Gecko::ComponentManager>();
    auto game = std::make_unique<Gecko::Game>(configuration);
    auto input = std::make_unique<Gecko::Input>(configuration);
    auto job_manager = std::make_unique<Gecko::JobManager>();
    auto map_manager = std::make_unique<Gecko::MapManager>();
    auto object_manager = std::make_unique<Gecko::ObjectManager>();
    auto order_manager = std::make_unique<Gecko::OrderManager>();
    auto player_manager = std::make_unique<Gecko::PlayerManager>();
    auto skill_manager = std::make_unique<Gecko::SkillManager>();
    auto statistics = std::make_unique<Gecko::Statistics>();
    auto technology_tree = std::make_unique<Gecko::TechnologyTree>();
    auto ui = std::make_unique<Gecko::UI>(configuration);

    // Initialize and run game.
    game->init();
    game->load_map("tutorial");
    
    auto main_window = game->get_window(Gecko::Settings::Window::MainName);

    input->set_render_window_handle(main_window->get_handle());
    input->init();

    main_window->on_resize();

    technology_tree->init();

    // TODO: Fix.
    // ui->init();

    {
        auto scene_manager = Gecko::Game::getSingleton().get_scene_manager();

        auto entity = scene_manager->createEntity("ogrehead.mesh");
        
        auto sceneNode = scene_manager->getRootSceneNode()->createChildSceneNode();
        sceneNode->attachObject(entity);
    }

    game->load_options();
    game->run();

    // TODO: Fix.
    // ui->deinit();

    input->deinit();

    game->deinit();

    return 0;
}
