#include "Gecko/Application.hpp"
#include "Gecko/Input.hpp"
#include "Gecko/Log.hpp"
#include "Gecko/Managers/ComponentManager.hpp"
#include "Gecko/Managers/ConfigurationManager.hpp"
#include "Gecko/Managers/JobManager.hpp"
#include "Gecko/Managers/MapManager.hpp"
#include "Gecko/Managers/OrderManager.hpp"
#include "Gecko/Managers/PlayerManager.hpp"
#include "Gecko/NVIDIA.hpp"
#include "Gecko/Statistics.hpp"
#include "Gecko/UI/UI.hpp"
#include "Gecko/Window.hpp"

#include "CityGame.hpp"
#include "CityObjectManager.hpp"

int main(int argc, char* argv[])
{
    Gecko::Log::setup();

    boost::program_options::variables_map options = Gecko::Application::parse_options(argc, argv, "city");
    std::string configuration_name = options["configuration"].as<std::string>();

    std::unique_ptr<Gecko::ConfigurationManager> configuration_manager = std::make_unique<Gecko::ConfigurationManager>();

    configuration_manager->parse_configuration_files();

    std::shared_ptr<Gecko::Configuration> configuration = configuration_manager->get(configuration_name);

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

    input->set_render_window_handle(game->get_window(Gecko::Settings::Window::MainName)->get_handle());
    input->init();

    game->get_window(Gecko::Settings::Window::MainName)->on_resize();

    ui->init();

    // Restore options.
    // TODO: Fix.
    // game->load_options();
    game->load_map("tutorial");
    game->run();

    // TODO: Enable cache.
    // configuration_manager->save_cache();

    return 0;
}
