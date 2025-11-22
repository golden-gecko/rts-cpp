#include "CityObjectManager.hpp"

namespace City
{
    Gecko::Id ObjectManager::get_empty_home()
    {
        /*
        auto buildings = Gecko::ObjectManager::getSingleton().get<CityBuilding>();

        for (const auto& [id, building] : buildings)
        {
            if (building->can_assign_resident() == true)
            {
                return building->get_id();
            }
        }

        L_WARNING << "No empty home found.";
        */

        return Gecko::Id::Empty;
    }

    Gecko::Id ObjectManager::get_empty_workplace()
    {
        /*
        auto buildings = Gecko::ObjectManager::getSingleton().get<CityBuilding>();

        for (const auto& [id, building] : buildings)
        {
            if (building->can_assign_worker() == true)
            {
                return building->get_id();
            }
        }

        L_WARNING << "No empty workplace found.";
        */

        return Gecko::Id::Empty;
    }

    Gecko::Id ObjectManager::get_building_with_resource(const std::string& resource_name, float resource_quantity)
    {
        /*
        auto buildings = Gecko::ObjectManager::getSingleton().get<CityBuilding>();

        for (const auto& [id, building] : buildings)
        {
            const auto& resources = building->get_resources();

            if (resources.has_resource(resource_name) == false)
            {
                continue;
            }

            const auto& resource = resources.get_resource(resource_name);

            if (resource.is_out() == false)
            {
                continue;
            }

            if (resource.get_current() < resource_quantity)
            {
                continue;
            }

            return id;
        }
        */

        return Gecko::Id::Empty;
    }

    Gecko::Id ObjectManager::get_building_without_resource(const std::string& resource_name, float resource_quantity)
    {
        /*
        auto buildings = Gecko::ObjectManager::getSingleton().get<CityBuilding>();

        for (const auto& [id, building] : buildings)
        {
            const auto& resources = building->get_resources();

            if (resources.has_resource(resource_name) == false)
            {
                continue;
            }

            const auto& resource = resources.get_resource(resource_name);

            if (resource.is_in() == false)
            {
                continue;
            }

            if (resource.get_max() - resource.get_current() < resource_quantity)
            {
                continue;
            }

            return id;
        }
        */

        return Gecko::Id::Empty;
    }

    std::pair<Gecko::Id, Citizen*> ObjectManager::get_citizen_without_orders()
    {
        /*
        auto citizens = Gecko::ObjectManager::getSingleton().get<Citizen>();

        for (const auto& [id, citizen] : citizens)
        {
            if (citizen->get_orders().size() == 0)
            {
                return std::make_pair(id, citizen);
            }
        }
        */

        return std::make_pair(Gecko::Id::Empty, nullptr);
    }
}
