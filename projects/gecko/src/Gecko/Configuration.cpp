#include "Gecko/Configuration.hpp"

#include "Gecko/Exception.hpp"
#include "Gecko/Utils/Convert.hpp"
#include "Gecko/Utils/Utils.hpp"

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

    void Configuration::append(const std::shared_ptr<Configuration>& value)
    {
        append<Json::Value>(value->m_value);
    }

    void Configuration::append(const std::string& path, const std::shared_ptr<Configuration>& value)
    {
        append<Json::Value>(path, value->m_value);
    }

    bool Configuration::has_member(const std::string& path) const
    {
        if (get_from_cache(path))
        {
            return true;
        }

        // OPTIMIZATION: If path has one segment then skip rest.
        if (path.find('.') == std::string::npos)
        {
            return m_value.isMember(path);
        }

        std::vector<std::string> segments;
        // TODO: Fix.
        // boost::algorithm::split(segments, path, boost::is_any_of("."), boost::token_compress_on);

        Json::Value current_value = m_value;

        for (const auto& i : segments)
        {
            if (current_value.isMember(i))
            {
                // TODO: Fix. Configuration::has_member with inexisting members is broken.
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
        // L_TRACE << "Configuration::load_includes()";

        auto includes = get_includes(*this);

        if (includes.empty() == false)
        {
            Json::Value merged;

            for (const auto& i : includes)
            {
                // L_INFO << "Parsing include " << i << ".";

                Configuration include(i);

                merge_json_objects(merged, include.m_value);
            }

            merge_json_objects(merged, m_value);

            merged.removeMember("include");

            m_value = merged;

            write_to_cache();
        }
    }

    void Configuration::merge(const std::shared_ptr<Configuration>& other)
    {
        // TODO: Make function const.
        merge_json_objects(m_value, other->m_value);
    }

    void Configuration::save_cache() const
    {
        // TODO: Create function json_to_file.
        using namespace std::filesystem;

        auto value_cache_path = get_value_cache_file_path();
        auto directory = path(value_cache_path).parent_path();

        if (exists(directory) == false)
        {
            create_directories(directory);
        }

        Utils::json_to_file(m_cache, value_cache_path);
    }

    std::string Configuration::to_string() const
    {
        return Utils::Convert::to_string(m_value);
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

    std::shared_ptr<Configuration> Configuration::get_child(const std::string& path) const
    {
        return std::make_shared<Configuration>(get(path));
    }

    std::optional<std::shared_ptr<Configuration>> Configuration::get_child_optional(const std::string& path) const
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

    std::shared_ptr<Configuration> Configuration::get_element(Json::Value::ArrayIndex index) const
    {
        // TODO: Add assert.
        return std::make_shared<Configuration>(m_value[index]);
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

    Path Configuration::get_path(const std::string& path) const
    {
        // TODO: Implement.
        return Path();
    }

    Path Configuration::get_path(const std::string& path, const Path& default_value) const
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

    void Configuration::set(const std::string& path, const std::shared_ptr<Configuration>& value)
    {
        set<Json::Value>(path, value->m_value);
    }

    // TODO: Add tests.
    void Configuration::set(const std::string& path, const Ogre::ColourValue& _value)
    {
        set<decltype(Ogre::ColourValue::r)>(path + ".r", _value.r);
        set<decltype(Ogre::ColourValue::g)>(path + ".g", _value.g);
        set<decltype(Ogre::ColourValue::b)>(path + ".b", _value.b);
        set<decltype(Ogre::ColourValue::a)>(path + ".a", _value.a);
    }

    // TODO: Add tests.
    void Configuration::set(const std::string& path, const Path& _value)
    {
        // TODO: Implement.
    }

    // TODO: Add tests.
    void Configuration::set(const std::string& path, const Ogre::Vector2& _value)
    {
        set<decltype(Ogre::Vector2::x)>(path + ".x", _value.x);
        set<decltype(Ogre::Vector2::y)>(path + ".y", _value.y);
    }

    // TODO: Add tests.
    void Configuration::set(const std::string& path, const Ogre::Vector3& _value)
    {
        set<decltype(Ogre::Vector3::x)>(path + ".x", _value.x);
        set<decltype(Ogre::Vector3::y)>(path + ".y", _value.y);
        set<decltype(Ogre::Vector3::z)>(path + ".z", _value.z);
    }

    // TODO: Add tests.
    void Configuration::set(const std::string& path, const std::map<std::string, float>& _value)
    {
        for (const auto& [key, value] : _value)
        {

        }
    }

    // TODO: Add tests.
    void Configuration::set(const std::string& path, const std::set<std::string>& _value)
    {
        for (const auto& i : _value)
        {
            append(path, i);
        }
    }

    // TODO: Add tests.
    void Configuration::set(const std::string& path, const std::vector<std::string>& _value)
    {
        for (const auto& i : _value)
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

        // OPTIMIZATION: If path has one segment then skip rest.
        if (path.find('.') == std::string::npos)
        {
            if (has_member(path) == false)
            {
                throw Exception("Configuration '" + m_name + "' has no '" + path + "' key.");
            }

            return m_value[path];
        }

        std::vector<std::string> segments;

        // TODO: Fix.
        // boost::algorithm::split(segments, path, boost::is_any_of("."), boost::token_compress_on);

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
        using namespace std::filesystem;

        // TODO: Move to configuration.
        auto cache_path
            = path("..")
            / path("cache")
            / path("configurations")
            ; // / boost::replace_all_copy(m_file_name, "/", "_");

        return cache_path.string();
    }

    std::string Configuration::get_value_cache_file_path() const
    {
        using namespace std::filesystem;

        // TODO: Move to configuration.
        auto cache_path
            = path("..")
            / path("cache")
            / path("values")
            ; // / boost::replace_all_copy(m_file_name, "/", "_");

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

                for (const auto& j : get_includes(Configuration(include_path)))
                {
                    includes.emplace_back(j);
                }
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

        // TODO: Remove or refactor.
        m_name = std::filesystem::path(m_file_name).filename().replace_extension("").string();

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
        // TODO: Make function const.
        merge_json_objects(m_value, other);
    }

    void Configuration::write_to_cache() const
    {
        using namespace std::filesystem;

        auto directory = path(get_cache_file_path()).parent_path();

        if (exists(directory) == false)
        {
            create_directories(directory);
        }

        Utils::json_to_file(m_value, get_cache_file_path(), true);
    }
}
