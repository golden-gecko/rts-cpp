#pragma once

namespace Gecko
{
    class Application
    {
    public:
        static boost::program_options::variables_map parse_options(int argc, char* argv[], const std::string& default_configuration);
    };
}
