#pragma once

#include "Gecko/Navigation/Path.hpp"

namespace Gecko
{
    class Configuration
    {
    public:
        explicit Configuration() = default;
        explicit Configuration(const char* file_name);
        explicit Configuration(const std::string& file_name);
        explicit Configuration(const Json::Value& value);

        virtual ~Configuration() = default;

    public:
        void append(const ConfigurationPtr& value);

        template<typename Type>
        void append(const Type& value)
        {
            m_value.append(value);
        }

        void append(const std::string& path, const ConfigurationPtr& value);

        template<typename Type>
        void append(const std::string& path, const Type& value)
        {
            // OPTIMIZATION: If path has one segment then skip rest.
            if (path.find('.') == std::string::npos)
            {
                m_value[path].append(value);

                return;
            }

            std::vector<std::string> segments;
            boost::algorithm::split(segments, path, boost::is_any_of("."), boost::token_compress_on);
            std::ranges::reverse(segments);

            Json::Value new_value;
            new_value[segments.front()].append(value);

            for (auto i = 1; i < segments.size(); i++)
            {
                Json::Value current_value;
                current_value[segments[i]] = new_value;
                new_value = current_value;
            }

            m_value = new_value;
        }

        bool has_member(const std::string& path) const;

        void load_includes();

        void merge(const ConfigurationPtr& other);

        void save_cache() const;

        auto size() const
        {
            return m_value.size();
        }

        std::string to_string(const std::string& indentation = "") const;

    public:
        bool get_bool() const;
        bool get_bool(const std::string& path) const;
        bool get_bool(const std::string& path, bool default_value) const;

        ConfigurationPtr get_child(const std::string& path) const;

        std::optional<ConfigurationPtr> get_child_optional(const std::string& path) const;

        Ogre::ColourValue get_color(const std::string& path) const;
        Ogre::ColourValue get_color(const std::string& path, const Ogre::ColourValue& default_value) const;

        ConfigurationPtr get_element(Json::Value::ArrayIndex index) const;

        float get_float() const;
        float get_float(const std::string& path) const;
        float get_float(const std::string& path, float default_value) const;

        template<typename Type = std::int64_t>
        Type get_int() const
        {
            return static_cast<Type>(m_value.asInt64());
        }

        template<typename Type = std::int64_t>
        Type get_int(const std::string& path) const
        {
            return static_cast<Type>(get(path).asInt64());
        }

        template<typename Type = std::int64_t>
        Type get_int(const std::string& path, Type default_value) const
        {
            if (has_member(path))
            {
                return get_int<Type>(path);
            }

            return default_value;
        }

        template<typename Key, typename Value>
        std::map<Key, Value> get_map(const std::string& path) const
        {
            std::map<Key, Value> map;

            auto child = get_child(path);

            for (auto i = child->begin(); i != child->end(); i++)
            {
                map.emplace(std::make_pair(i.key().asString(), i->as<Value>()));
            }

            return map;
        }

        template<typename Key, typename Value>
        std::map<Key, Value> get_map(const std::string& path, const std::map<Key, Value>& default_value) const
        {
            if (has_member(path))
            {
                return get_map<Key, Value>(path);
            }

            return default_value;
        }

        Navigation::Path get_path(const std::string& path) const;
        Navigation::Path get_path(const std::string& path, const Navigation::Path& default_value) const;

        std::string get_string() const;
        std::string get_string(const std::string& path) const;
        std::string get_string(const std::string& path, const std::string& default_value) const;

        template<typename Type = std::vector<std::string>>
        Type get_string_array(const std::string& path) const
        {
            Type v;

            if (has_member(path))
            {
                for (const auto& i : get(path))
                {
                    v.emplace(i.asString());
                }
            }

            return v;
        }

        template<typename Type = std::vector<std::string>>
        Type get_string_array(const std::string& path, const Type& default_value) const
        {
            if (has_member(path))
            {
                return get_string_array<Type>(path);
            }

            return default_value;
        }

        Ogre::Vector2 get_vector2(const std::string& path) const;
        Ogre::Vector2 get_vector2(const std::string& path, const Ogre::Vector2& default_value) const;

        Ogre::Vector3 get_vector3(const std::string& path) const;
        Ogre::Vector3 get_vector3(const std::string& path, const Ogre::Vector3& default_value) const;

        const std::string& get_name() const
        {
            return m_name;
        }

        const std::string& get_file_name() const
        {
            return m_file_name;
        }

    public:
        void set(const std::string& path, const ConfigurationPtr& value);
        void set(const std::string& path, const Ogre::ColourValue& value);
        void set(const std::string& path, const Navigation::Path& value);
        void set(const std::string& path, const Ogre::Vector2& value);
        void set(const std::string& path, const Ogre::Vector3& value);
        void set(const std::string& path, const std::map<std::string, float>& value);
        void set(const std::string& path, const std::set<std::string>& value);
        void set(const std::string& path, const std::vector<std::string>& value);

        template<typename Type>
        void set(const std::string& path, const Type& value)
        {
            // OPTIMIZATION: If path has one segment then skip rest.
            if (path.find('.') == std::string::npos)
            {
                m_value[path] = value;

                return;
            }

            std::vector<std::string> segments;
            boost::algorithm::split(segments, path, boost::is_any_of("."), boost::token_compress_on);
            std::ranges::reverse(segments);

            Json::Value new_value;
            new_value[segments.front()] = value;

            for (auto i = 1; i < segments.size(); i++)
            {
                Json::Value current_value;
                current_value[segments[i]] = new_value;
                new_value = current_value;
            }

            merge(new_value);
        }

        void set_name(const std::string& name)
        {
            m_name = name;
        }

    public:
        Json::Value::iterator begin()
        {
            return m_value.begin();
        }

        Json::Value::iterator end()
        {
            return m_value.end();
        }

        Json::Value::const_iterator begin() const
        {
            return m_value.begin();
        }

        Json::Value::const_iterator end() const
        {
            return m_value.end();
        }

    private:
        std::string m_file_name;
        std::string m_name;

        Json::Value m_value;

        mutable Json::Value m_cache;

        void add_to_cache(const std::string& path, const Json::Value& value) const;

        Json::Value get(const std::string& path) const;

        std::string get_cache_file_path() const;

        std::optional<Json::Value> get_from_cache(const std::string& path) const;

        std::vector<std::string> get_includes(const Configuration& value) const;

        std::string get_value_cache_file_path() const;

        bool is_cached() const;

        void load_from_file(const std::string& _file_name);

        void merge(const Json::Value& other);
    };
}
