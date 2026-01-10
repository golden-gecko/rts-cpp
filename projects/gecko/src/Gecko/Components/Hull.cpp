#include "Gecko/Components/Hull.hpp"

#include "Gecko/Configuration.hpp"

namespace Gecko
{
    Hull* Hull::create(Hull* memory, const ConfigurationPtr& configuration)
    {
        auto component = new (memory) Hull();

        component->deserialize(configuration);

        return component;
    }
}
