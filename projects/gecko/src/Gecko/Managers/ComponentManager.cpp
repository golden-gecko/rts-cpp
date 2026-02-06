#include "Gecko/Managers/ComponentManager.hpp"

#include "Gecko/Components/Armour.hpp"
#include "Gecko/Components/Debug.hpp"
#include "Gecko/Components/Drive.hpp"
#include "Gecko/Components/Hull.hpp"
#include "Gecko/Components/Radar.hpp"
#include "Gecko/Components/Shield.hpp"
#include "Gecko/Components/Storage.hpp"
#include "Gecko/Components/Weapon.hpp"
#include "Gecko/Configuration.hpp"
#include "Gecko/Log.hpp"
#include "Gecko/Managers/ConfigurationManager.hpp"
#include "Gecko/Utils/Time.hpp"

Gecko::ComponentManager* Ogre::Singleton<Gecko::ComponentManager>::msSingleton = nullptr;

namespace Gecko
{
    void ComponentManager::init(const ConfigurationPtr& configuration)
    {
        L_TIME("ComponentManager::init()");

        auto max_size = configuration->get_int<std::size_t>("memory.components");
        auto& component_manager = ComponentManager::getSingleton();

        for (const auto& [name, configuration] : ConfigurationManager::getSingleton())
        {
            L_INFO << "Loading '" << name << "' configuration.";

            auto type = configuration->get_string("type", "");

            if (type == "Armour")
            {
                auto factory = std::bind(Armour::create, std::placeholders::_1, configuration);

                component_manager.register_type<Armour>(name, factory);
                component_manager.allocate(name, max_size);
            }
            else if (type == "Debug")
            {
                auto factory = std::bind(Debug::create, std::placeholders::_1, configuration);

                component_manager.register_type<Debug>(name, factory);
                component_manager.allocate(name, max_size);
            }
            else if (type == "Drive")
            {
                auto factory = std::bind(Drive::create, std::placeholders::_1, configuration);

                component_manager.register_type<Drive>(name, factory);
                component_manager.allocate(name, max_size);
            }
            else if (type == "Hull")
            {
                auto factory = std::bind(Hull::create, std::placeholders::_1, configuration);

                component_manager.register_type<Hull>(name, factory);
                component_manager.allocate(name, max_size);
            }
            else if (type == "Radar")
            {
                auto factory = std::bind(Radar::create, std::placeholders::_1, configuration);

                component_manager.register_type<Radar>(name, factory);
                component_manager.allocate(name, max_size);
            }
            else if (type == "Shield")
            {
                auto factory = std::bind(Shield::create, std::placeholders::_1, configuration);

                component_manager.register_type<Shield>(name, factory);
                component_manager.allocate(name, max_size);
            }
            else if (type == "Storage")
            {
                auto factory = std::bind(Storage::create, std::placeholders::_1, configuration);

                component_manager.register_type<Storage>(name, factory);
                component_manager.allocate(name, max_size);
            }
            else if (type == "Weapon")
            {
                auto factory = std::bind(Weapon::create, std::placeholders::_1, configuration);

                component_manager.register_type<Weapon>(name, factory);
                component_manager.allocate(name, max_size);
            }
        }
    }

    void ComponentManager::deinit()
    {
        deallocate();
    }
}
