#include "Gecko/Managers/ComponentManager.hpp"

#include "Gecko/Components/Armour.hpp"
#include "Gecko/Components/Debug.hpp"
#include "Gecko/Components/Drive.hpp"
#include "Gecko/Components/Hull.hpp"
#include "Gecko/Components/Producer.hpp"
#include "Gecko/Components/Radar.hpp"
#include "Gecko/Components/Shield.hpp"
#include "Gecko/Components/Storage.hpp"
#include "Gecko/Components/Weapon.hpp"
#include "Gecko/Configuration.hpp"
#include "Gecko/Exception.hpp"
#include "Gecko/Log.hpp"
#include "Gecko/Managers/ConfigurationManager.hpp"
#include "Gecko/Utils/Time.hpp"

Gecko::ComponentManager* Ogre::Singleton<Gecko::ComponentManager>::msSingleton = nullptr;

namespace Gecko
{
    void ComponentManager::init()
    {
        L_TIME("ComponentManager::init()");

        std::uint64_t max_size = m_configuration->get_int<std::uint64_t>("memory.components");

        for (const auto& [name, configuration] : ConfigurationManager::getSingleton())
        {
            L_INFO << "Loading '" << name << "' configuration.";

            std::string type = configuration->get_string("type", "");

            if (type == "Armour")
            {
                register_type<Armour>(name, std::bind(Armour::create, std::placeholders::_1, configuration, m_scene));
            }
            else if (type == "Debug")
            {
                register_type<Debug>(name, std::bind(Debug::create, std::placeholders::_1, configuration, m_scene));
            }
            else if (type == "Drive")
            {
                register_type<Drive>(name, std::bind(Drive::create, std::placeholders::_1, configuration, m_scene));
            }
            else if (type == "Hull")
            {
                register_type<Hull>(name, std::bind(Hull::create, std::placeholders::_1, configuration, m_scene));
            }
            else if (type == "Producer")
            {
                register_type<Producer>(name, std::bind(Producer::create, std::placeholders::_1, configuration, m_scene));
            }
            else if (type == "Radar")
            {
                register_type<Radar>(name, std::bind(Radar::create, std::placeholders::_1, configuration, m_scene));
            }
            else if (type == "Shield")
            {
                register_type<Shield>(name, std::bind(Shield::create, std::placeholders::_1, configuration, m_scene));
            }
            else if (type == "Storage")
            {
                register_type<Storage>(name, std::bind(Storage::create, std::placeholders::_1, configuration, m_scene));
            }
            else if (type == "Weapon")
            {
                register_type<Weapon>(name, std::bind(Weapon::create, std::placeholders::_1, configuration, m_scene));
            }

            allocate(name, max_size);
        }
    }

    void ComponentManager::deinit()
    {
        unregister_all();
    }

    ComponentManager::ComponentManager(const ConfigurationPtr& configuration, const ScenePtr& scene) :
        m_configuration(configuration),
        m_scene(scene)
    {
    }
}
