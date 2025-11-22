#include "Containers/Configurations.hpp"

#include "Configuration.hpp"

namespace Gecko
{
    std::shared_ptr<Configuration> Configurations::serialize() const
    {
        auto configuration = std::make_shared<Configuration>();

        for (const auto& i : available)
        {
            configuration->append(i);
        }

        return configuration;
    }

    void Configurations::deserialize(const std::shared_ptr<Configuration>& configuration)
    {
        clear();

        for (const auto& i : *(configuration))
        {
            available.emplace(i.asString());
        }
    }
}
