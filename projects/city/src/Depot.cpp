#include "Depot.hpp"

namespace City
{
    Depot::Depot(const Depot& other) :
        base_type(other)
    {
        maxTrucks = other.maxTrucks;
    }

    void Depot::init()
    {
        base_type::init();

        /*
        for (int i = 0; i < maxTrucks; i++)
        {
            // Create truck.
            const auto object = Gecko::ObjectManager::getSingleton().create("truck");
            const auto vehicle = Gecko::ObjectManager::getSingleton().get<Vehicle>(object->get_id());

            // Add truck to list.
            trucks.emplace_back(vehicle);
        }
        */
    }

    Vehicle* Depot::getAvailableTruck() const
    {
        /*
        for (const auto& truck : trucks)
        {
            if (truck->get_state() == Gecko::State::Value::None)
            {
                return truck;
            }
        }
        */

        return nullptr;
    }

    int Depot::getAvailableTrucksCount() const
    {
        int count = 0;

        /*
        for (const auto& truck : trucks)
        {
            if (truck->get_state() == Gecko::State::Value::None)
            {
                count++;
            }
        }
        */

        return count;
    }

    void Depot::set_position(const Ogre::Vector3& position, bool validate)
    {
        base_type::set_position(position);

        /*
        // Park truck in the front of the depot.
        auto bEntrance = get_entrance();

        if (bEntrance)
        {
            for (auto truck: trucks)
            {
                // TODO: Is this really needed? Can building move after being created on the map?
                if (truck->get_state() == Gecko::State::Value::None)
                {
                    truck->set_position(bEntrance->position);
                    truck->set_direction(bEntrance->direction.crossProduct(Ogre::Vector3::NEGATIVE_UNIT_Y));
                }
            }
        }
        else
        {
            L_WARNING << "One or more of target buildings have no entrance.";
        }
        */
    }
}
