#pragma once

#include "Gecko/Interfaces/Serializable.hpp"

namespace Gecko
{
    class Diplomacy :
        public Ogre::Singleton<Diplomacy>,
        public Serializable
    {
    public:
        enum class State : // TODO: Move.
            std::int8_t
        {
            Ally,
            Neutral,
            Enemy
        };

    public:
        static State from_name(const std::string& name); // TODO: Move.
        static std::string to_name(State state); // TODO: Move.

    // From Serializable.
    public:
        ConfigurationPtr serialize() const override;
        void deserialize(const ConfigurationPtr& configuration) override;

    // Diplomacy.
    public:
        State get(ObjectPtr object_a, ObjectPtr object_b) const;
        State get(PlayerPtr player_a, PlayerPtr player_b) const;

        State get(ObjectPtr object, PlayerPtr player) const;
        State get(PlayerPtr player, ObjectPtr object) const;

        const std::map<std::string, std::map<std::string, State>>& get_states() const
        {
            return m_states;
        }

    private:
        std::map<std::string, std::map<std::string, State>> m_states;
    };
}
