#include "Gecko/Managers/MapManager.hpp"

#include "Gecko/Maps/Map.hpp"

template<> Gecko::MapManager* Ogre::Singleton<Gecko::MapManager>::msSingleton = nullptr;

namespace Gecko
{
    void MapManager::update(float time)
    {
        auto update = [](Map& map, float time)
        {
            map.update(time);
        };

        iterate(std::bind(update, std::placeholders::_1, time));
    }
}
