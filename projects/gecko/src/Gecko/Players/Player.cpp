#include "Gecko/Players/Player.hpp"

#include "Gecko/Configuration.hpp"
#include "Gecko/Containers/Diplomacy.hpp"
#include "Gecko/Containers/Resources.hpp"
#include "Gecko/Containers/Selected.hpp"
#include "Gecko/Managers/ObjectManager.hpp"
#include "Gecko/Objects/Object.hpp"

namespace Gecko
{
    Player* Player::create()
    {
        return new Player();
    }

    Player* Player::create(Player* memory)
    {
        return new (memory) Player();
    }

    Player* Player::create(const ConfigurationPtr& configuration)
    {
        auto player = new Player();

        player->deserialize(configuration);

        return player;
    }

    Player* Player::create(Player* memory, const ConfigurationPtr& configuration)
    {
        auto player = new (memory) Player();

        player->deserialize(configuration);

        return player;
    }

    Player::Player()
    {
        m_diplomacy = std::make_shared<Diplomacy>();
        m_resources = std::make_shared<Resources>();
        m_selected = std::make_shared<Selected>();
    }

    Player::Player(const Player& other) :
        base_type(other)
    {
        m_name = other.m_name;
        m_color = other.m_color;

        if (other.m_diplomacy)
        {
            m_diplomacy = std::make_shared<Diplomacy>(*(other.m_diplomacy.get()));
        }

        if (other.m_resources)
        {
            m_resources = std::make_shared<Resources>(*(other.m_resources.get()));
        }

        if (other.m_selected)
        {
            m_selected = std::make_shared<Selected>(*(other.m_selected.get()));
        }
    }

    ConfigurationPtr Player::serialize() const
    {
        auto configuration = base_type::serialize();

        configuration->set("name", m_name);
        configuration->set("color", m_color);

        return configuration;
    }

    void Player::deserialize(const ConfigurationPtr& configuration)
    {
        base_type::deserialize(configuration);

        m_name = configuration->get_string("name");
        m_color = configuration->get_string("color");
    }

    void Player::update(float time)
    {
        m_resources->clear();

        for (const auto& [_, object] : ObjectManager::getSingleton())
        {
            if (get_id() == object->get_player_id())
            {
                // TODO: Replace with merge method.
                // TODO: Does not work because player resources are not initialized.
                for (const auto& [_, resource] : (*object->get_resources().get()))
                {
                    m_resources->add(resource.get_name(), resource.get_current());
                }
            }
        }
    }
}
