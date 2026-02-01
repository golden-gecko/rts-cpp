#pragma once

namespace Gecko
{
    class Application
    {
    public:
        static boost::program_options::variables_map parse_options(int argc, char* argv[], const std::string& default_configuration, const std::string& default_map);

        static std::string get_option(const boost::program_options::variables_map& options, const std::string& name);
    };
}
