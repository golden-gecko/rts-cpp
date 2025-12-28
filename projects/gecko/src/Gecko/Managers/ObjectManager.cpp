#include "Gecko/Managers/ObjectManager.hpp"

#include "Gecko/Objects/Object.hpp"
#include "Gecko/Statistics.hpp"

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
