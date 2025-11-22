#pragma once

#include "Objects/Object.hpp"

namespace Gecko
{
    class Plant : public Object
    {
    private:
        using base_type = Object;

    public:
        static Plant* create(Plant* memory, std::shared_ptr<Configuration> configuration);

    public:
        Plant();
        Plant(const Plant& other);
    };
}
