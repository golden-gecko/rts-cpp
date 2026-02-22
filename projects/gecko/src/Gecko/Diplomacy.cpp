#include "Gecko/Diplomacy.hpp"

#include "Gecko/Configuration.hpp"
#include "Gecko/Managers/PlayerManager.hpp"
#include "Gecko/Objects/Object.hpp"
#include "Gecko/Players/Player.hpp"

Gecko::Diplomacy* Ogre::Singleton<Gecko::Diplomacy>::msSingleton = nullptr;

namespace Gecko
{
    Diplomacy::State Diplomacy::from_name(const std::string& name)
    {
        static const std::map<std::string, State> types =
        {
            { "Ally" , State::Ally  },
            { "Neutral" , State::Neutral  },
            { "Enemy", State::Enemy }
        };

        auto type = types.find(name);

        if (type != types.end())
        {
            return type->second;
        }

        return State::Neutral;
    }

    std::string Diplomacy::to_name(State state)
    {
        static const std::map<State, std::string> types =
        {
            { State::Ally, "Ally" },
            { State::Neutral, "Neutral" },
            { State::Enemy, "Enemy" }
        };

        auto type = types.find(state);

        if (type != types.end())
        {
            return type->second;
        }

        return "Neutral";
    }

    ConfigurationPtr Diplomacy::serialize() const
    {
        return std::make_shared<Configuration>();
    }

    void Diplomacy::deserialize(const ConfigurationPtr& configuration)
    {
        for (auto i = configuration->begin(); i != configuration->end(); i++)
        {
            std::string player_a = i.key().asString();

            for (auto j = i->begin(); j != i->end(); j++)
            {
                std::string player_b = j.key().asString();

                m_states[player_a][player_b] = from_name(j->asString());
                m_states[player_b][player_a] = from_name(j->asString());

                m_states[player_b][player_b] = State::Ally;
            }

            m_states[player_a][player_a] = State::Ally;
        }
    }

    Diplomacy::State Diplomacy::get(ObjectPtr object_a, ObjectPtr object_b) const
    {
        if (PlayerPtr player_a = PlayerManager::getSingleton().get(object_a->get_player_id()))
        {
            if (PlayerPtr player_b = PlayerManager::getSingleton().get(object_b->get_player_id()))
            {
                return get(player_a, player_b);
            }
        }

        return State::Neutral;
    }

    Diplomacy::State Diplomacy::get(PlayerPtr player_a, PlayerPtr player_b) const
    {
        auto state_a = m_states.find(player_a->get_configuration()->get_name());

        if (state_a == m_states.end())
        {
            return State::Neutral;
        }

        auto state_b = state_a->second.find(player_b->get_configuration()->get_name());

        if (state_b == state_a->second.end())
        {
            return State::Neutral;
        }

        return state_b->second;
    }

    Diplomacy::State Diplomacy::get(ObjectPtr object, PlayerPtr player) const
    {
        if (PlayerPtr player_a = PlayerManager::getSingleton().get(object->get_player_id()))
        {
            return get(player_a, player);
        }

        return State::Neutral;
    }

    Diplomacy::State Diplomacy::get(PlayerPtr player, ObjectPtr object) const
    {
        return get(object, player);
    }
}
