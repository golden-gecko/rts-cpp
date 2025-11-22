#include "Objects/Building.hpp"

namespace Gecko
{
    Building* Building::create(Building* memory, std::shared_ptr<Configuration> configuration)
    {
        auto map = new (memory) Building();

        map->deserialize(configuration);

        return map;
    }

    Building::Building()
    {
    }

    Building::Building(const Building& other) :
        base_type(other)
    {
    }
}
