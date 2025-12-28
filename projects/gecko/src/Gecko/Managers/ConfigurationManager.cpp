#include "Gecko/Managers/ConfigurationManager.hpp"

#include "Gecko/Exception.hpp"
#include "Gecko/Log.hpp"
#include "Gecko/Settings.hpp"

Gecko::ConfigurationManager* Ogre::Singleton<Gecko::ConfigurationManager>::msSingleton = nullptr;

namespace Gecko
{
    void ConfigurationManager::parse_configuration_files()
    {
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
            const auto& path = i->path();
            auto path_string = path.generic_string();
            auto name = path.stem().string();

            L_INFO << "Parsing path '" << path_string << "'.";

            if (is_directory(path))
            {
                parse_directory(path_string);

                continue;
            }

            if (path.extension() != Settings::Configuration::Extension)
            {
                L_WARNING << "File '" << path_string << "' has invalid extension.";

                continue;
            }

            if (configurations.find(name) != configurations.end())
            {
                throw Exception("Configuration '" + name + "' from '" + path_string + "' already exists.");
            }

            auto configuration = std::make_shared<Configuration>(path_string);

            if (configuration->get_bool("creatable.by_game", false) == false)
            {
                L_WARNING << "Configuration '" << name << "' not creatable by game.";

                continue;
            }

            configurations.emplace(name, configuration);
        }
    }

    void ConfigurationManager::save_cache() const
    {
        for (const auto& [path, configuration] : configurations)
        {
            configuration->save_cache();
        }
    }

    std::shared_ptr<Configuration> ConfigurationManager::get(const std::string& name) const
    {
        auto configuration = configurations.find(name);

        if (configuration == configurations.end())
        {
            throw Exception("No configuration with name '" + name + "'.");
        }

        return configuration->second;
    }
}
