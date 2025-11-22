#include "Objects/Plant.hpp"

namespace Gecko
{
    Plant* Plant::create(Plant* memory, std::shared_ptr<Configuration> configuration)
    {
        auto map = new (memory) Plant();

        map->deserialize(configuration);

        return map;
    }

    Plant::Plant()
    {
    }

    Plant::Plant(const Plant& other) :
        base_type(other)
    {
    }
}
