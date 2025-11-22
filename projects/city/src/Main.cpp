#include "Application.hpp"
#include "CityGame.hpp"
#include "CityObjectManager.hpp"
#include "Input.hpp"
#include "Log.hpp"
#include "Managers/ComponentManager.hpp"
#include "Managers/ConfigurationManager.hpp"
#include "Managers/JobManager.hpp"
#include "Managers/MapManager.hpp"
#include "Managers/OrderManager.hpp"
#include "Managers/PlayerManager.hpp"
#include "NVIDIA.hpp"
#include "Statistics.hpp"
#include "UI.hpp"
#include "Window.hpp"

int main(int argc, char* argv[])
{
    /*
    try
    {
    */
        Gecko::Log::setup();

        auto options = Gecko::Application::parse_options(argc, argv, "city");
        const auto& configuration_name = options["configuration"].as<std::string>();

        auto configuration_manager = std::make_unique<Gecko::ConfigurationManager>();
        configuration_manager->parse_configuration_files();

        auto configuration = configuration_manager->get(configuration_name);

        auto component_manager = std::make_unique<Gecko::ComponentManager>();
        auto game = std::make_unique<Gecko::Game>(configuration);
        auto input = std::make_unique<Gecko::Input>(configuration);
        auto job_manager = std::make_unique<Gecko::JobManager>();
        auto map_manager = std::make_unique<Gecko::MapManager>();
        auto object_manager = std::make_unique<Gecko::ObjectManager>();
        auto order_manager = std::make_unique<Gecko::OrderManager>();
        auto player_manager = std::make_unique<Gecko::PlayerManager>();
        auto statistics = std::make_unique<Gecko::Statistics>();
        auto ui = std::make_unique<Gecko::UI>(configuration);

        game->init();

        input->init(game->get_window(Gecko::Settings::Window::MainName)->get_handle());

        game->get_window(Gecko::Settings::Window::MainName)->on_resize();

        ui->init();

        // TODO: Fix.
        // Restore options.
        // game->load_options();
        game->load_map("tutorial");
        game->run();

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
