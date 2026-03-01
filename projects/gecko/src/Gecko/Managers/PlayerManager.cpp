#include "Gecko/Managers/PlayerManager.hpp"

#include "Gecko/Configuration.hpp"
#include "Gecko/Log.hpp"
#include "Gecko/Managers/ConfigurationManager.hpp"
#include "Gecko/Players/Player.hpp"
#include "Gecko/Utils/Time.hpp"

Gecko::PlayerManager* Ogre::Singleton<Gecko::PlayerManager>::msSingleton = nullptr;

namespace Gecko
{
    void PlayerManager::init()
    {
        L_TIME("PlayerManager::init()");

        Size max_size = m_configuration->get_int<Size>("memory.players");

        for (const auto& [name, configuration] : ConfigurationManager::getSingleton())
        {
            L_INFO << "Loading '" << name << "' configuration.";

            std::string type = configuration->get_string("type", "");

            if (type == Player::Name)
            {
                register_type<Player>(name, std::bind(Player::create, std::placeholders::_1, configuration));
            }

            allocate(name, max_size);
        }
    }

    void PlayerManager::deinit()
    {
        unregister_all();
    }

    void PlayerManager::update(float time)
    {
        auto update = [time](Player& player)
        {
            player.update(time);   
        };

        iterate(std::bind(update, std::placeholders::_1));
    }

    PlayerManager::PlayerManager(const ConfigurationPtr& configuration) :
        m_configuration(configuration)
    {
    }

    PlayerPtr PlayerManager::get_by_configuration_name(const std::string& name) const
    {
        auto result = std::find_if(cbegin(), cend(), [name](const Items::value_type& player) {
            return player.second->get_configuration()->get_name() == name;
        });

        if (result == cend())
        {
            return nullptr;
        }

        return result->second;
    }
}
