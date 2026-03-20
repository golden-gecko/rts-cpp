#pragma once

namespace Gecko
{
    class Application
    {
    public:
        using Options = boost::program_options::variables_map;

    public:
        static Options parse_options(int argc, char* argv[], const std::map<std::string, std::string>& defaults);

        static std::string get_option(const Options& options, const std::string& name);
    };
}
