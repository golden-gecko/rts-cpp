#include "Gecko/Managers/ObjectManager.hpp"

#include "Gecko/Configuration.hpp"
#include "Gecko/Log.hpp"
#include "Gecko/Managers/ConfigurationManager.hpp"
#include "Gecko/Objects/Factory.hpp"
#include "Gecko/Objects/Missile.hpp"
#include "Gecko/Objects/Vehicle.hpp"
#include "Gecko/Utils/Time.hpp"

Gecko::ObjectManager* Ogre::Singleton<Gecko::ObjectManager>::msSingleton = nullptr;

namespace Gecko
{
    void ObjectManager::init()
    {
        L_TIME("ObjectManager::init()");

        std::uint64_t max_size = m_configuration->get_int<std::uint64_t>("memory.objects");

        for (const auto& [name, configuration] : ConfigurationManager::getSingleton())
        {
            L_INFO << "Loading '" << name << "' configuration.";

            std::string type = configuration->get_string("type", "");

            if (type == "Factory")
            {
                register_type<Factory>(name, std::bind(Factory::create, std::placeholders::_1, configuration, m_scene));
            }
            else if (type == "Missile")
            {
                register_type<Missile>(name, std::bind(Missile::create, std::placeholders::_1, configuration, m_scene));
            }
            else if (type == "Object")
            {
                register_type<Object>(name, std::bind(Object::create, std::placeholders::_1, configuration, m_scene));
            }
            else if (type == "Vehicle")
            {
                register_type<Vehicle>(name, std::bind(Vehicle::create, std::placeholders::_1, configuration, m_scene));
            }

            allocate(name, max_size);
        }
    }

    void ObjectManager::deinit()
    {
        unregister_all();
    }

    void ObjectManager::update(float time)
    {
        auto update = [time](Object& object)
        {
            object.update(time);
        };

        iterate(std::bind(update, std::placeholders::_1));
    }

    ObjectManager::ObjectManager(const ConfigurationPtr& configuration, const ScenePtr& scene) :
        m_configuration(configuration),
        m_scene(scene)
    {
    }

    ObjectPtr ObjectManager::create(const std::string& name)
    {
        ObjectPtr object = base_type::create(name);

        if (object)
        {
            // Statistics::getSingleton().add("Objects created", 1.0f); TODO: Restore.
        }

        return object;
    }

    ObjectManager::ObjectsInRange ObjectManager::get_in_range(const Ogre::Vector3& position, float range)
    {
        ObjectsInRange objects_in_range;

        auto get_objects_in_range = [&](Object& object)
        {
            auto distance = object.get_position().distance(position);

            if (object.get_position().distance(position) <= range)
            {
                objects_in_range.emplace_back(std::make_pair(&object, distance));
            }
        };

        iterate(std::bind(get_objects_in_range, std::placeholders::_1));

        std::ranges::sort(objects_in_range, [&](const auto& a, const auto& b)
            {
                return a.first->get_position().squaredDistance(position) < b.first->get_position().squaredDistance(position);
            }
        );

        return objects_in_range;
    }
}
