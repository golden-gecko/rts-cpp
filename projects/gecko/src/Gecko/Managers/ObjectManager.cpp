#include "Gecko/Managers/ObjectManager.hpp"

#include "Gecko/Configuration.hpp"
#include "Gecko/Log.hpp"
#include "Gecko/Managers/ConfigurationManager.hpp"
#include "Gecko/Objects/Factory.hpp"
#include "Gecko/Objects/Missile.hpp"
#include "Gecko/Objects/Vehicle.hpp"
#include "Gecko/Statistics.hpp"
#include "Gecko/Utils/Time.hpp"

Gecko::ObjectManager* Ogre::Singleton<Gecko::ObjectManager>::msSingleton = nullptr;

namespace Gecko
{
    Object* ObjectManager::create(const std::string& name)
    {
        auto object = base_type::create(name);

        if (object)
        {
            Statistics::getSingleton().add("Objects created", 1.0f);
        }

        return object;
    }

    void ObjectManager::update(float time)
    {
        auto update = [](Object& object, float time)
        {
            object.update(time);
        };

        iterate(std::bind(update, std::placeholders::_1, time));
    }

    void ObjectManager::init(const ConfigurationPtr& configuration)
    {
        L_TIME("ObjectManager::init()");

        auto max_size = configuration->get_int<std::size_t>("memory.objects");
        auto& object_manager = ObjectManager::getSingleton();

        for (const auto& [name, configuration] : ConfigurationManager::getSingleton())
        {
            L_INFO << "Loading '" << name << "' configuration.";

            // TODO: Remove default value.
            auto type = configuration->get_string("type", "");

            if (type == "Factory")
            {
                auto factory = std::bind(Factory::create, std::placeholders::_1, configuration);

                object_manager.register_type<Factory>(name, factory);
                object_manager.allocate(name, max_size);
            }
            else if (type == "Missile")
            {
                auto factory = std::bind(Missile::create, std::placeholders::_1, configuration);

                object_manager.register_type<Missile>(name, factory);
                object_manager.allocate(name, max_size);
            }
            else if (type == "Object")
            {
                auto factory = std::bind(Object::create, std::placeholders::_1, configuration);

                object_manager.register_type<Object>(name, factory);
                object_manager.allocate(name, max_size);
            }
            else if (type == "Vehicle")
            {
                auto factory = std::bind(Vehicle::create, std::placeholders::_1, configuration);

                object_manager.register_type<Vehicle>(name, factory);
                object_manager.allocate(name, max_size);
            }
        }
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
