#include "Gecko/Components/Storage.hpp"

namespace Gecko
{
    Storage* Storage::create(Storage* memory, const ConfigurationPtr& configuration, const ScenePtr& scene)
    {
        auto component = new (memory) Storage();

        component->deserialize(configuration);

        return component;
    }
}
