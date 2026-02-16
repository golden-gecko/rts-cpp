#include "Gecko/Components/Radar.hpp"

namespace Gecko
{
    Radar* Radar::create(Radar* memory, const ConfigurationPtr& configuration, const ScenePtr& scene)
    {
        auto component = new (memory) Radar();

        component->deserialize(configuration);

        return component;
    }
}
