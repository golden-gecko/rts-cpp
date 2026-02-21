#include "Gecko/Utils/File.hpp"

namespace Gecko::Utils::File
{
    std::string get_file_name_from_current_date()
    {
        std::time_t now = std::time(nullptr);
        std::tm* local_now = std::localtime(&now);

        std::ostringstream oss;

        oss << std::put_time(local_now, "%Y_%m_%d_%H_%M_%S");

        return oss.str();
    }

    std::string get_file_name_no_extension(const std::string& name)
    {
        return std::filesystem::path(name).filename().replace_extension("").string();
    }

    void json_to_file(const Json::Value& value, const std::string& file_name, bool minify)
    {
        std::ofstream stream(file_name, std::ifstream::binary);

        stream << json_to_string(value, minify);
    }

    std::string json_to_string(const Json::Value& value, bool minify)
    {
        Json::StreamWriterBuilder builder;

        builder["commentStyle"] = "None";
        builder["indentation"] = "";

        return Json::writeString(builder, value);
    }

    std::string to_file_name(const std::string& value)
    {
        return boost::algorithm::to_lower_copy(value);
    }
}
