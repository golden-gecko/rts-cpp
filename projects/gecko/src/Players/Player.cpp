#include "Player.hpp"

#include "Configuration.hpp"
#include "Containers/Diplomacy.hpp"
#include "Containers/Resources.hpp"
#include "Containers/Selected.hpp"

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

    Player* Player::create(const std::shared_ptr<Configuration>& configuration)
    {
        auto player = new Player();

        player->deserialize(configuration);

        return player;
    }

    Player* Player::create(Player* memory, const std::shared_ptr<Configuration>& configuration)
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

    std::shared_ptr<Configuration> Player::serialize() const
    {
        auto configuration = base_type::serialize();

        configuration->set("name", m_name);
        configuration->set("color", m_color);

        return configuration;
    }

    void Player::deserialize(const std::shared_ptr<Configuration>& configuration)
    {
        base_type::deserialize(configuration);

        m_name = configuration->get_string("name");
        m_color = configuration->get_string("color");
    }

    void Player::update(float time)
    {
    }
}
