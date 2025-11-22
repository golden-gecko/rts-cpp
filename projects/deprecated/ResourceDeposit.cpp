#include "Objects/ResourceDeposit.hpp"

namespace Gecko
{
    ResourceDeposit* ResourceDeposit::create(ResourceDeposit* memory, std::shared_ptr<Configuration> configuration)
    {
        auto map = new (memory) ResourceDeposit();

        map->deserialize(configuration);

        return map;
    }

    ResourceDeposit::ResourceDeposit()
    {
    }

    ResourceDeposit::ResourceDeposit(const ResourceDeposit& other) :
        base_type(other)
    {
    }
}
