#pragma once

#include "Gecko/Managers/ObjectManager.hpp"

namespace City
{
    class Citizen;

    class ObjectManager : public Gecko::ObjectManager
    {
    public:
        Gecko::Id get_empty_home();
        Gecko::Id get_empty_workplace();

        Gecko::Id get_building_with_resource(const std::string& resource_name, float resource_quantity);
        Gecko::Id get_building_without_resource(const std::string& resource_name, float resource_quantity);

        std::pair<Gecko::Id, Citizen*> get_citizen_without_orders();
    };
}
