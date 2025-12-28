#include "Gecko/Application.hpp"

namespace Gecko
{
    boost::program_options::variables_map Application::parse_options(int argc, char* argv[], const std::string& default_configuration)
    {
        boost::program_options::options_description options;
        options.add_options()("configuration", boost::program_options::value<std::string>()->default_value(default_configuration));

        boost::program_options::variables_map vm;
        store(parse_command_line(argc, argv, options), vm);
        notify(vm);

        return vm;
    }
}
