#include "Gecko/Diplomacy.hpp"

#include "Gecko/Configuration.hpp"

namespace Gecko
{
    Diplomacy::State Diplomacy::from_name(const std::string& name)
    {
        static const std::map<std::string, State> types =
        {
            { "Ally" , State::Ally  },
            { "Enemy", State::Enemy }
        };

        auto type = types.find(name);

        if (type != types.end())
        {
            return type->second;
        }

        return State::Enemy;
    }

    ConfigurationPtr Diplomacy::serialize() const
    {
        return std::make_shared<Configuration>();
    }

    void Diplomacy::deserialize(const ConfigurationPtr& configuration)
    {
    }
}
