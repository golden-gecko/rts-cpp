#include "Gecko/Players/Player.hpp"

#include "Gecko/Configuration.hpp"
#include "Gecko/Containers/Processes.hpp"
#include "Gecko/Containers/Resources.hpp"
#include "Gecko/Containers/Selected.hpp"
#include "Gecko/Managers/ObjectManager.hpp"
#include "Gecko/Objects/Object.hpp"

namespace Gecko
{
    PlayerPtr Player::create(PlayerPtr memory, const ConfigurationPtr& configuration)
    {
        auto player = new (memory) Player();

        player->deserialize(configuration);

        return player;
    }

    Player::Player()
    {
        m_resources = std::make_shared<Resources>();
        m_selected = std::make_shared<Selected>();
    }

    Player::Player(const Player& other) :
        base_type(other)
    {
        m_name = other.m_name;
        m_color = other.m_color;

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
        ConfigurationPtr configuration = base_type::serialize();

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
                m_resources->merge(*(object->get_resources().get()));

                for (const auto& [_, process] : *(object->get_processes().get()))
                {
                    m_resources->merge(*(process.get_in().get()));
                    m_resources->merge(*(process.get_out().get()));
                }
            }
        }
    }

    Id Player::get_first_selected()
    {
        if (m_selected->size() > 0)
        {
            return *(m_selected->begin());
        }

        return Id::Empty;
    }
}
