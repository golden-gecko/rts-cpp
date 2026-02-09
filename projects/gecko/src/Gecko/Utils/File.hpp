#pragma once

namespace Gecko::Utils::File
{
    std::string get_name_no_extension(const std::string& name);

    void json_to_file(const Json::Value& value, const std::string& file_name, bool minify = false);

    std::string json_to_string(const Json::Value& value, bool minify);

    std::string to_file_name(const std::string& value);
}
