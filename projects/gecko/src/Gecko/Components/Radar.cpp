#include "Gecko/Components/Radar.hpp"

namespace Gecko
{
    Radar* Radar::create(Radar* memory, const std::shared_ptr<Configuration>& configuration)
    {
        auto component = new (memory) Radar();

        component->deserialize(configuration);

        return component;
    }
}
