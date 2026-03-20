#include "Gecko/Application.hpp"

namespace Gecko
{
    Application::Options Application::parse_options(int argc, char* argv[], const std::map<std::string, std::string>& defaults)
    {
        boost::program_options::options_description options;

        for (const auto& [key, value] : defaults)
        {
            options.add_options()(key.c_str(), boost::program_options::value<std::string>()->default_value(value));
        }

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
