#include "Containers/Diplomacy.hpp"

#include "Configuration.hpp"

namespace Gecko
{
    Diplomacy::State Diplomacy::from_name(const std::string& name)
    {
        static const std::map<std::string, State> types =
        {
            { "Ally", State::Ally },
            { "Enemy", State::Enemy }
        };

        auto type = types.find(name);

        if (type != types.end())
        {
            return type->second;
        }

        return State::Enemy;
    }

    std::shared_ptr<Configuration> Diplomacy::serialize() const
    {
        auto configuration = std::make_shared<Configuration>();

        // TODO: Implement.

        return configuration;
    }

    void Diplomacy::deserialize(const std::shared_ptr<Configuration>& configuration)
    {
        // clear();

        // TODO: Implement.
    }
}
