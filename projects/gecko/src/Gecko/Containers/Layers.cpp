#include "Gecko/Containers/Layers.hpp"

#include "Gecko/Configuration.hpp"

namespace Gecko
{
    std::shared_ptr<Configuration> Layers::serialize() const
    {
        auto configuration = std::make_shared<Configuration>();

        for (const auto& i : m_items)
        {
            configuration->append(i);
        }

        return configuration;
    }

    void Layers::deserialize(const std::shared_ptr<Configuration>& configuration)
    {
        clear();

        for (const auto& i : *(configuration))
        {
            m_items.emplace(i.asString());
        }
    }
}
