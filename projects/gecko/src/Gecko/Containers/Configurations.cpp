#include "Gecko/Containers/Configurations.hpp"

#include "Gecko/Configuration.hpp"

namespace Gecko
{
    ConfigurationPtr Configurations::serialize() const
    {
        auto configuration = std::make_shared<Configuration>();

        for (const auto& i : available)
        {
            configuration->append(i);
        }

        return configuration;
    }

    void Configurations::deserialize(const ConfigurationPtr& configuration)
    {
        clear();

        for (const auto& i : *(configuration))
        {
            available.emplace(i.asString());
        }
    }
}
