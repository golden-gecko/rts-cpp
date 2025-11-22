#pragma once

#include "Objects/Object.hpp"

namespace Gecko
{
    class ResourceDeposit : public Object
    {
    private:
        using base_type = Object;

    public:
        static ResourceDeposit* create(ResourceDeposit* memory, std::shared_ptr<Configuration> configuration);

    public:
        ResourceDeposit();
        ResourceDeposit(const ResourceDeposit& other);
    };
}
