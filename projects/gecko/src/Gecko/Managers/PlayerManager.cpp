#include "Gecko/Managers/PlayerManager.hpp"

#include "Gecko/Configuration.hpp"
#include "Gecko/Log.hpp"
#include "Gecko/Managers/ConfigurationManager.hpp"
#include "Gecko/Players/Player.hpp"
#include "Gecko/Utils/Time.hpp"

Gecko::PlayerManager* Ogre::Singleton<Gecko::PlayerManager>::msSingleton = nullptr;

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

    void PlayerManager::init(const ConfigurationPtr& configuration)
    {
        L_TIME("PlayerManager::init()");

        auto max_size = configuration->get_int<std::size_t>("memory.players");
        auto& player_manager = PlayerManager::getSingleton();

        for (const auto& [name, configuration] : ConfigurationManager::getSingleton())
        {
            L_INFO << "Loading '" << name << "' configuration.";

            // TODO: Remove default value.
            auto type = configuration->get_string("type", "");

            if (type == "Player")
            {
                auto function = static_cast<Player*(*)(Player* memory, const ConfigurationPtr&)>(&Player::create);
                auto factory = std::bind(function, std::placeholders::_1, configuration);

                player_manager.register_type<Player>(name, factory);
                player_manager.allocate(name, max_size);
            }
        }
    }
}
