#include "Gecko/Managers/PlayerManager.hpp"

#include "Gecko/Players/Player.hpp"

template<> Gecko::PlayerManager* Ogre::Singleton<Gecko::PlayerManager>::msSingleton = nullptr;

namespace Gecko
{
    void PlayerManager::update(float time)
    {
        auto update = [](Player& player, float time)
        {
            player.update(time);   
        };

        iterate(std::bind(update, std::placeholders::_1, time));
    }
}
