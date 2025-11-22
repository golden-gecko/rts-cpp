#include "Components/Hull.hpp"

#include "Configuration.hpp"

namespace Gecko
{
    Hull* Hull::create(Hull* memory, const std::shared_ptr<Configuration>& configuration)
    {
        auto component = new (memory) Hull();

        component->deserialize(configuration);

        return component;
    }
}
