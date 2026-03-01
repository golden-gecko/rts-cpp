#pragma once

namespace Gecko
{
    class Application
    {
    public:
        using Options = boost::program_options::variables_map;

    public:
        static Options parse_options(int argc, char* argv[], const std::string& default_configuration, const std::string& default_map);

        static std::string get_option(const Options& options, const std::string& name);
    };
}
