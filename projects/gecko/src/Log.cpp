#include "Log.hpp"

namespace Gecko
{
    void Log::setup(const std::string& file_name, int max_size)
    {
        using namespace boost::log;

        add_console_log(std::cout);

        add_file_log(
            keywords::file_name = file_name,
            keywords::rotation_size = max_size
        );

        core::get()->set_filter(
            trivial::severity >= trivial::debug
        );
    }
}
