#pragma once

#include <Gecko/Maps/Map.hpp>

namespace Gecko
{
    class Configuration;
}

namespace City
{
    class CityBuilding;

    class Map : public Gecko::Map
    {
    private:
        using base_type = Gecko::Map;

    public:
        static Map* create(std::shared_ptr<Gecko::Configuration> configuration);
        static Map* create(Map* memory, std::shared_ptr<Gecko::Configuration> configuration);

    public:
        explicit Map() = default;
        explicit Map(const Map& other) = default;

        void init_objects() override;

    private:
        void init_buildings();
        void init_citizens(CityBuilding* home);
        void init_vehicles(CityBuilding* home);

        void create_building(std::shared_ptr<Gecko::Layer> layer, bool is_industrial, bool is_residential, std::size_t x, std::size_t z, std::size_t size_x, std::size_t size_z);
    };
}
