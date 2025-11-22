#pragma once

#include "Object.hpp"

namespace Gecko
{
    class Building : public Object
    {
    private:
        using base_type = Object;

    public:
        static Building* create(Building* memory, std::shared_ptr<Configuration> configuration);

    public:
        Building();
        Building(const Building& other);
    };
}
