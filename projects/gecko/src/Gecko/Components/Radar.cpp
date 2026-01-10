#include "Gecko/Components/Radar.hpp"

namespace Gecko
{
    Radar* Radar::create(Radar* memory, const ConfigurationPtr& configuration)
    {
        auto component = new (memory) Radar();

        component->deserialize(configuration);

        return component;
    }
}
