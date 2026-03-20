#include "Gecko/Configuration.hpp"

#include "Gecko/Exception.hpp"
#include "Gecko/Settings.hpp"
#include "Gecko/Utils/Convert.hpp"
#include "Gecko/Utils/File.hpp"

namespace
{
    void merge_json_objects(Json::Value& super, const Json::Value& sub)
    {
        for (const auto& key : sub.getMemberNames())
        {
            if (super[key].type() == Json::objectValue && sub[key].type() == Json::objectValue)
            {
                merge_json_objects(super[key], sub[key]);
            }
            else
            {
                super[key] = sub[key];
            }
        }
    }
}

namespace Gecko
{
    Configuration::Configuration(const char* file_name)
    {
        load_from_file(file_name);
    }

    Configuration::Configuration(const std::string& file_name)
    {
        load_from_file(file_name);
    }

    Configuration::Configuration(const Json::Value& value) :
        m_value(value)
    {
    }

    void Configuration::append(const ConfigurationPtr& value)
    {
        append<Json::Value>(value->m_value);
    }

    void Configuration::append(const std::string& path, const ConfigurationPtr& value)
    {
        append<Json::Value>(path, value->m_value);
    }

    bool Configuration::has_member(const std::string& path) const
    {
        if (get_from_cache(path))
        {
            return true;
        }

        // If path has one segment then skip rest.
        if (path.find('.') == std::string::npos)
        {
            return m_value.isMember(path);
        }

        std::vector<std::string> segments;
        boost::algorithm::split(segments, path, boost::is_any_of("."), boost::token_compress_on);

        Json::Value current_value = m_value;

        for (const auto& i : segments)
        {
            if (current_value.isMember(i))
            {
                current_value = current_value[i];
            }
            else
            {
                return false;
            }
        }

        return true;
    }

    void Configuration::load_includes()
    {
        auto includes = get_includes(*this);

        if (includes.empty() == false)
        {
            Json::Value merged;

            for (const auto& i : includes)
            {
                Configuration include(i);

                merge_json_objects(merged, include.m_value);
            }

            merge_json_objects(merged, m_value);

            merged.removeMember("include");

            m_value = merged;
        }
    }

    void Configuration::merge(const ConfigurationPtr& other)
    {
        merge_json_objects(m_value, other->m_value);
    }

    void Configuration::save_cache() const
    {
        std::string cache_path = get_cache_file_path();
        std::filesystem::path directory = std::filesystem::path(cache_path).parent_path();

        if (std::filesystem::exists(directory) == false)
        {
            std::filesystem::create_directories(directory);
        }

        Utils::File::json_to_file(m_value, cache_path, true);
    }

    std::string Configuration::to_string(const std::string& indentation) const
    {
        return Utils::Convert::to_string(m_value, indentation);
    }

    bool Configuration::get_bool() const
    {
        return m_value.asBool();
    }

    bool Configuration::get_bool(const std::string& path) const
    {
        return get(path).asBool();
    }

    bool Configuration::get_bool(const std::string& path, bool default_value) const
    {
        if (has_member(path))
        {
            return get_bool(path);
        }

        return default_value;
    }

    ConfigurationPtr Configuration::get_child(const std::string& path) const
    {
        return std::make_shared<Configuration>(get(path));
    }

    std::optional<ConfigurationPtr> Configuration::get_child_optional(const std::string& path) const
    {
        if (has_member(path))
        {
            return get_child(path);
        }

        return {};
    }

    Ogre::ColourValue Configuration::get_color(const std::string& path) const
    {
        return Ogre::ColourValue(
            get_float(path + ".r"),
            get_float(path + ".g"),
            get_float(path + ".b"),
            get_float(path + ".a")
        );
    }

    Ogre::ColourValue Configuration::get_color(const std::string& path, const Ogre::ColourValue& default_value) const
    {
        return Ogre::ColourValue(
            get_float(path + ".r", default_value.r),
            get_float(path + ".g", default_value.b),
            get_float(path + ".b", default_value.b),
            get_float(path + ".a", default_value.a)
        );
    }

    ConfigurationPtr Configuration::get_element(Json::Value::ArrayIndex index) const
    {
        if (index < m_value.size())
        {
            return std::make_shared<Configuration>(m_value[index]);
        }

        return {};
    }

    float Configuration::get_float() const
    {
        return m_value.asFloat();
    }

    float Configuration::get_float(const std::string& path) const
    {
        return get(path).asFloat();
    }

    float Configuration::get_float(const std::string& path, float default_value) const
    {
        if (has_member(path))
        {
            return get_float(path);
        }

        return default_value;
    }

    Navigation::Path Configuration::get_path(const std::string& path) const
    {
        Navigation::Path::Points points;

        for (const auto& i : *(get_child(path)))
        {
            points.push_back(Navigation::Coordinate(
                Configuration(i).get_int<decltype(Navigation::Coordinate::x)>("x"),
                Configuration(i).get_int<decltype(Navigation::Coordinate::y)>("y"),
                Configuration(i).get_int<decltype(Navigation::Coordinate::z)>("z")
            ));
        }

        return Navigation::Path(points);
    }

    Navigation::Path Configuration::get_path(const std::string& path, const Navigation::Path& default_value) const
    {
        if (has_member(path))
        {
            return get_path(path);
        }

        return default_value;
    }

    std::string Configuration::get_string() const
    {
        return m_value.asString();
    }

    std::string Configuration::get_string(const std::string& path) const
    {
        return get(path).asString();
    }

    std::string Configuration::get_string(const std::string& path, const std::string& default_value) const
    {
        if (has_member(path))
        {
            return get_string(path);
        }

        return default_value;
    }

    std::optional<std::string> Configuration::get_string_optional(const std::string& path) const
    {
        if (has_member(path))
        {
            return get_string(path);
        }

        return {};
    }

    Ogre::Vector2 Configuration::get_vector2(const std::string& path) const
    {
        return Ogre::Vector2(
            get_float(path + ".x"),
            get_float(path + ".y")
        );
    }

    Ogre::Vector2 Configuration::get_vector2(const std::string& path, const Ogre::Vector2& default_value) const
    {
        return Ogre::Vector2(
            get_float(path + ".x", default_value.x),
            get_float(path + ".y", default_value.y)
        );
    }

    Ogre::Vector3 Configuration::get_vector3(const std::string& path) const
    {
        return Ogre::Vector3(
            get_float(path + ".x"),
            get_float(path + ".y"),
            get_float(path + ".z")
        );
    }

    Ogre::Vector3 Configuration::get_vector3(const std::string& path, const Ogre::Vector3& default_value) const
    {
        return Ogre::Vector3(
            get_float(path + ".x", default_value.x),
            get_float(path + ".y", default_value.y),
            get_float(path + ".z", default_value.z)
        );
    }

    void Configuration::set(const std::string& path, const ConfigurationPtr& value)
    {
        set<Json::Value>(path, value->m_value);
    }

    void Configuration::set(const std::string& path, const Ogre::ColourValue& value)
    {
        set<decltype(Ogre::ColourValue::r)>(path + ".r", value.r);
        set<decltype(Ogre::ColourValue::g)>(path + ".g", value.g);
        set<decltype(Ogre::ColourValue::b)>(path + ".b", value.b);
        set<decltype(Ogre::ColourValue::a)>(path + ".a", value.a);
    }

    void Configuration::set(const std::string& path, const Navigation::Path& value)
    {
        for (const auto& i : value.get_points())
        {
            Configuration point;

            point.set("x", i.x);
            point.set("y", i.y);
            point.set("z", i.z);

            append(path, point.m_value);
        }
    }

    void Configuration::set(const std::string& path, const Ogre::Vector2& value)
    {
        set<decltype(Ogre::Vector2::x)>(path + ".x", value.x);
        set<decltype(Ogre::Vector2::y)>(path + ".y", value.y);
    }

    void Configuration::set(const std::string& path, const Ogre::Vector3& value)
    {
        set<decltype(Ogre::Vector3::x)>(path + ".x", value.x);
        set<decltype(Ogre::Vector3::y)>(path + ".y", value.y);
        set<decltype(Ogre::Vector3::z)>(path + ".z", value.z);
    }

    void Configuration::set(const std::string& path, const std::map<std::string, float>& value)
    {
        for (const auto& [key, _value] : value)
        {
            set(path + "." + key, _value);
        }
    }

    void Configuration::set(const std::string& path, const std::set<std::string>& value)
    {
        for (const auto& i : value)
        {
            append(path, i);
        }
    }

    void Configuration::set(const std::string& path, const std::vector<std::string>& value)
    {
        for (const auto& i : value)
        {
            append(path, i);
        }
    }

    void Configuration::add_to_cache(const std::string& path, const Json::Value& value) const
    {
        m_cache[path] = value;
    }

    Json::Value Configuration::get(const std::string& path) const
    {
        auto value_from_cache = get_from_cache(path);

        if (value_from_cache)
        {
            return value_from_cache.value();
        }

        // If path has one segment then skip rest.
        if (path.find('.') == std::string::npos)
        {
            if (has_member(path) == false)
            {
                throw Exception("Configuration '" + m_name + "' has no '" + path + "' key.");
            }

            return m_value[path];
        }

        std::vector<std::string> segments;
        boost::algorithm::split(segments, path, boost::is_any_of("."), boost::token_compress_on);

        Json::Value return_value = m_value;

        for (const auto& i : segments)
        {
            if (return_value.isMember(i))
            {
                return_value = return_value[i];
            }
            else
            {
                throw Exception("Configuration '" + get_name() + "' has no '" + path + "' key.");
            }
        }

        add_to_cache(path, return_value);

        return return_value;
    }

    std::string Configuration::get_cache_file_path() const
    {
        std::filesystem::path cache_path
            = std::filesystem::path(Settings::Cache::ConfigurationsPath)
            / boost::replace_all_copy(m_file_name, "/", "_");

        return cache_path.string();
    }

    std::string Configuration::get_value_cache_file_path() const
    {
        std::filesystem::path cache_path
            = std::filesystem::path(Settings::Cache::ValuesPath)
            / boost::replace_all_copy(m_file_name, "/", "_");

        return cache_path.string();
    }

    std::optional<Json::Value> Configuration::get_from_cache(const std::string& path) const
    {
        if (m_cache.isMember(path))
        {
            return m_cache[path];
        }

        return {};
    }

    std::vector<std::string> Configuration::get_includes(const Configuration& value) const
    {
        std::vector<std::string> includes;

        auto includes_child = value.get_child_optional("include");

        if (includes_child)
        {
            for (const auto& i : *(includes_child.value()))
            {
                auto include_path = i.asString();

                includes.emplace_back(include_path);

                auto includes_from_child = get_includes(Configuration(include_path));

                std::copy(includes_from_child.begin(), includes_from_child.end(), includes.begin());
            }
        }

        return includes;
    }

    bool Configuration::is_cached() const
    {
        return std::filesystem::exists(get_cache_file_path());
    }

    void Configuration::load_from_file(const std::string& file_name)
    {
        m_file_name = file_name;

        std::string new_file_name;

        if (is_cached())
        {
            new_file_name = get_cache_file_path();
        }
        else
        {
            new_file_name = m_file_name;
        }

        m_name = Utils::File::get_file_name_no_extension(m_file_name);

        std::ifstream stream(new_file_name, std::ifstream::binary);
        Json::CharReaderBuilder builder;
        std::string errors;

        if (Json::parseFromStream(builder, stream, &m_value, &errors) == false)
        {
            throw Exception("Failed to parse file '" + new_file_name + "'.");
        }

        load_includes();
    }

    void Configuration::merge(const Json::Value& other)
    {
        merge_json_objects(m_value, other);
    }
}
