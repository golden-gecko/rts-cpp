#pragma once

#include "Gecko/Interfaces/Serializable.hpp"

namespace Gecko
{
    class Diplomacy :
        public Serializable
    {
    public:
        enum class State :
            std::int8_t
        {
            Ally,
            Neutral,
            Enemy
        };

    public:
        static State from_name(const std::string& name);

    public:
        // From Serializable.
        ConfigurationPtr serialize() const override;
        void deserialize(const ConfigurationPtr& configuration) override;
    };
}
