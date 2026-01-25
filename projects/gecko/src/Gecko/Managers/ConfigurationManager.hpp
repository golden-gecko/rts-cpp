#pragma once

namespace Gecko
{
    class ConfigurationManager :
        public Ogre::Singleton<ConfigurationManager>
    {
    public:
        using Map = std::map<std::string, ConfigurationPtr>;

    public:
        explicit ConfigurationManager(bool cache = true);

        void parse_configuration_files(const std::vector<std::string>& directories);
        void parse_directory(const std::string& directory);

        void save_cache() const;

    public:
        ConfigurationPtr get(const std::string& name) const;
        const Map& get_configurations() const;

    public:
        Map::iterator begin();
        Map::iterator end();

        Map::const_iterator cbegin() const;
        Map::const_iterator cend() const;

    private:
        Map m_configurations;

        bool m_cache = true;
    };
}
