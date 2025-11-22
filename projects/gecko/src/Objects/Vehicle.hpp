#pragma once

#include "Objects/Object.hpp"

#include "Timer.hpp"

namespace Gecko
{
    class Vehicle : public Object
    {
    private:
        using base_type = Object;

    public:
        static Vehicle* create(Vehicle* memory, const std::shared_ptr<Configuration>& configuration);

    public:
        explicit Vehicle() = default;
        explicit Vehicle(const Vehicle& other) = default;

    protected:
        order_status on_attack(Order* order, float time) override;
        order_status on_load(Order* order, float time) override;
        order_status on_move(Order* order, float time) override;
        order_status on_unload(Order* order, float time) override;
    };
}
