#pragma once

#include "CityBuilding.hpp"

namespace City
{
    class Vehicle;

    class Depot : public CityBuilding
    {
    private:
        using base_type = CityBuilding;

    public:
        explicit Depot() = default;
        explicit Depot(const Depot& other);

        void init() override;

    public:
        Vehicle* getAvailableTruck() const;

        int getAvailableTrucksCount() const;

    public:
        void set_position(const Ogre::Vector3& position, bool validate = true) override;

    private:
        std::size_t maxTrucks = 0;
        std::vector<Gecko::Id> trucks;
    };
}
