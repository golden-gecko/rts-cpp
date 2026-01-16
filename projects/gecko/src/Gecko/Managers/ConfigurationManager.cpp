#include "Gecko/Managers/ConfigurationManager.hpp"

#include "Gecko/Configuration.hpp"
#include "Gecko/Exception.hpp"
#include "Gecko/Log.hpp"
#include "Gecko/Settings.hpp"
#include "Gecko/Utils/Time.hpp"

Gecko::ConfigurationManager* Ogre::Singleton<Gecko::ConfigurationManager>::msSingleton = nullptr;

namespace Gecko
{
    ConfigurationManager::ConfigurationManager(bool cache) :
        m_cache(cache)
    {
    }

    void ConfigurationManager::parse_configuration_files()
    {
        L_TIME("Gecko::ConfigurationManager::parse_configuration_files()");

        for (const std::string& directory : Settings::Configuration::Directories)
        {
            parse_directory(directory);
        }
    }

    void ConfigurationManager::parse_directory(const std::string& directory)
    {
        L_INFO << "Parsing directory '" << directory << "'.";

        for (auto i = std::filesystem::directory_iterator(directory); i != std::filesystem::directory_iterator(); i++)
        {
            std::string path = i->path().generic_string();
            std::string name = i->path().stem().generic_string();

            L_INFO << "Parsing path '" << path << "'.";

            if (std::filesystem::is_directory(i->path()))
            {
                parse_directory(path);

                continue;
            }

            if (i->path().extension() != Settings::Configuration::Extension)
            {
                L_WARNING << "File '" << path << "' has invalid extension.";

                continue;
            }

            if (m_configurations.find(name) != m_configurations.end())
            {
                throw Exception("Configuration '" + name + "' from '" + path + "' already exists.");
            }

            ConfigurationPtr configuration = std::make_shared<Configuration>(path);

            if (configuration->get_bool("creatable.by_game", false) == false)
            {
                L_WARNING << "Configuration '" << name << "' not creatable by game.";

                continue;
            }

            m_configurations.emplace(name, configuration);
        }
    }

    void ConfigurationManager::save_cache() const
    {
        if (m_cache)
        {
            for (const auto& [path, configuration] : m_configurations)
            {
                configuration->save_cache();
            }
        }
    }

    ConfigurationPtr ConfigurationManager::get(const std::string& name) const
    {
        Map::const_iterator configuration = m_configurations.find(name);

        if (configuration == m_configurations.end())
        {
            throw Exception("No configuration with name '" + name + "'.");
        }

        return configuration->second;
    }

    const ConfigurationManager::Map& ConfigurationManager::get_configurations() const
    {
        return m_configurations;
    }

    ConfigurationManager::Map::iterator ConfigurationManager::begin()
    {
        return m_configurations.begin();
    }

    ConfigurationManager::Map::iterator ConfigurationManager::end()
    {
        return m_configurations.end();
    }

    ConfigurationManager::Map::const_iterator ConfigurationManager::cbegin() const
    {
        return m_configurations.cbegin();
    }

    ConfigurationManager::Map::const_iterator ConfigurationManager::cend() const
    {
        return m_configurations.cend();
    }
}
