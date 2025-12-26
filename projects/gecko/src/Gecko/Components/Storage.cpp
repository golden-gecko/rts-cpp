#include "Gecko/Components/Storage.hpp"

namespace Gecko
{
    Storage* Storage::create(Storage* memory, const std::shared_ptr<Configuration>& configuration)
    {
        auto component = new (memory) Storage();

        component->deserialize(configuration);

        return component;
    }
}
