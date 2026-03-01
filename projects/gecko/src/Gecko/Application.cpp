#include "Gecko/Application.hpp"

namespace Gecko
{
    Application::Options Application::parse_options(int argc, char* argv[], const std::string& default_configuration, const std::string& default_map)
    {
        boost::program_options::options_description options;

        options.add_options()("configuration", boost::program_options::value<std::string>()->default_value(default_configuration));
        options.add_options()("map", boost::program_options::value<std::string>()->default_value(default_map));

        Options vm;

        boost::program_options::store(boost::program_options::parse_command_line(argc, argv, options), vm);
        boost::program_options::notify(vm);

        return vm;
    }

    std::string Application::get_option(const Options& options, const std::string& name)
    {
        return options[name].as<std::string>();
    }
}
