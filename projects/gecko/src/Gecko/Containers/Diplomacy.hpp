#pragma once

#include "Gecko/Interfaces/Serializable.hpp"

namespace Gecko
{
    class Diplomacy :
        public Serializable
    {
    public:
        // From Serializable.
        ConfigurationPtr serialize() const override;
        void deserialize(const ConfigurationPtr& configuration) override;

    public:
        enum class State :
            std::int8_t
        {
            Ally,
            Enemy
        };

        static State from_name(const std::string& name);
    };
}
