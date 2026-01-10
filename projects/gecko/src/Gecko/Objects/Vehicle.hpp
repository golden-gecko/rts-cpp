#pragma once

#include "Gecko/Objects/Object.hpp"

#include "Gecko/Timer.hpp"

namespace Gecko
{
    class Vehicle :
        public Object
    {
    private:
        using base_type = Object;

    public:
        static Vehicle* create(Vehicle* memory, const ConfigurationPtr& configuration);

    public:
        explicit Vehicle() = default;
        explicit Vehicle(const Vehicle& other) = default;

    protected:
        OrderStatus on_attack(Order* order, float time) override;
        OrderStatus on_load(Order* order, float time) override;
        OrderStatus on_move(Order* order, float time) override;
        OrderStatus on_unload(Order* order, float time) override;
    };
}
