#include "Gecko/Players/Player.hpp"

#include "Gecko/Configuration.hpp"
#include "Gecko/Containers/Processes.hpp"
#include "Gecko/Containers/Resources.hpp"
#include "Gecko/Containers/Selection.hpp"
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
        m_selected = std::make_shared<Selection>();
    }

    /*Player::Player(const Player& other) :
        base_type(other)
    {
        m_name  = other.m_name;
        m_color = other.m_color;

        m_resources = std::make_shared<Resources>(*(other.m_resources.get()));
        m_selected  = std::make_shared<Selection>(*(other.m_selected.get()));
    }*/

    ConfigurationPtr Player::serialize() const
    {
        ConfigurationPtr configuration = base_type::serialize();

        configuration->set("name", m_name);
        configuration->set("color", m_color);

        // TODO: Add m_resources.
        // TODO: Add m_selected.

        return configuration;
    }

    void Player::deserialize(const ConfigurationPtr& configuration)
    {
        base_type::deserialize(configuration);

        m_name  = configuration->get_string("name");
        m_color = configuration->get_string("color");

        // TODO: Add m_resources.
        // TODO: Add m_selected.
    }

    void Player::update(float time)
    {
        m_resources->clear();

        for (const auto& object : ObjectManager::getSingleton().get_by_player(get_id()))
        {
            m_resources->merge(*(object->get_resources().get()));
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
