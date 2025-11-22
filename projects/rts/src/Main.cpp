#include "Application.hpp"
#include "Games/Game.hpp"
#include "Input.hpp"
#include "Log.hpp"
#include "Managers/ComponentManager.hpp"
#include "Managers/ConfigurationManager.hpp"
#include "Managers/JobManager.hpp"
#include "Managers/MapManager.hpp"
#include "Managers/ObjectManager.hpp"
#include "Managers/OrderManager.hpp"
#include "Managers/PlayerManager.hpp"
#include "Managers/SkillManager.hpp"
#include "NVIDIA.hpp"
#include "Statistics.hpp"
#include "Technologies/TechnologyTree.hpp"
#include "UI/UI.hpp"
#include "Window.hpp"

int main(int argc, char* argv[])
{
    /*
    try
    {
    */
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

        ui->init();

        game->load_options();
        game->run();

        ui->deinit();

        input->deinit();

        game->deinit();

        // TODO: Restore.
        // configuration_manager->save_cache();
    /*
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << " Exiting..." << std::endl;
    }
    catch (...)
    {
        std::cout << "Unknown error. Exiting..." << std::endl;
    }
    */

    return 0;
}
