#pragma once

#define L_TRACE   BOOST_LOG_TRIVIAL(trace)
#define L_DEBUG   BOOST_LOG_TRIVIAL(debug)
#define L_INFO    BOOST_LOG_TRIVIAL(info)
#define L_WARNING BOOST_LOG_TRIVIAL(warning)
#define L_ERROR   BOOST_LOG_TRIVIAL(error)
#define L_FATAL   BOOST_LOG_TRIVIAL(fatal)

namespace Gecko
{
    class Log
    {
    public:
        static void setup(const std::string& file_name = "Gecko.log", int max_size = 10 * 1024 * 1024, bool write_to_console = true, bool write_to_file = true);
    };
}
