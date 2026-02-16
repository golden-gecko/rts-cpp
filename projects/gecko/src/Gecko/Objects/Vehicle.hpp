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
        static Vehicle* create(Vehicle* memory, const ConfigurationPtr& configuration, const ScenePtr& scene);

    public:
        explicit Vehicle(const ScenePtr& scene);
        explicit Vehicle(const Vehicle& other);

    protected:
        OrderStatus on_attack(Order* order, float time) override;
        OrderStatus on_load(Order* order, float time) override;
        OrderStatus on_move(Order* order, float time) override;
        OrderStatus on_unload(Order* order, float time) override;
    };
}
