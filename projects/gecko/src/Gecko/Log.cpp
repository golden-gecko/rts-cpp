#include "Gecko/Log.hpp"

namespace Gecko
{
    void Log::setup(const std::string& file_name, int max_size, bool write_to_console, bool write_to_file)
    {
        if (write_to_console)
        {
            boost::log::add_console_log(
                std::cout
            );
        }

        if (write_to_file)
        {
            boost::log::add_file_log(
                boost::log::keywords::file_name = file_name,
                boost::log::keywords::rotation_size = max_size
            );
        }

        boost::log::core::get()->set_filter(
            boost::log::trivial::severity >= boost::log::trivial::trace
        );
    }
}
