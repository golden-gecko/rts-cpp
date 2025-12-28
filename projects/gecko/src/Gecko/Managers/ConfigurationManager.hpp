#pragma once

#include "Gecko/Configuration.hpp"

namespace Gecko
{
    class ConfigurationManager : public Ogre::Singleton<ConfigurationManager>
    {
    public:
        void parse_configuration_files();

        void parse_directory(const std::string& directory);

        void save_cache() const;

    public:
        std::shared_ptr<Configuration> get(const std::string& name) const;

        const auto& get_configurations() const
        {
            return configurations;
        }

    public:
        auto begin() const
        {
            return configurations.begin();
        }

        auto end() const
        {
            return configurations.end();
        }

        auto cbegin() const
        {
            return configurations.cbegin();
        }

        auto cend() const
        {
            return configurations.cend();
        }

    private:
        std::map<std::string, std::shared_ptr<Configuration>> configurations;
    };
}
