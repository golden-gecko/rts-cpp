#include "Gecko/Components/Hull.hpp"

#include "Gecko/Configuration.hpp"

namespace Gecko
{
    Hull* Hull::create(Hull* memory, const std::shared_ptr<Configuration>& configuration)
    {
        auto component = new (memory) Hull();

        component->deserialize(configuration);

        return component;
    }
}
