#include <gtest/gtest.h>

#include <Gecko/Configuration.hpp>
#include <Gecko/Managers/MapManager.hpp>
#include <Gecko/Maps/Map.hpp>

TEST(map_manager, new_map_manager_is_empty)
{
    auto map_manager = std::make_shared<Gecko::MapManager>();

    EXPECT_EQ(map_manager->is_type_registered("map"), false);
}

TEST(map_manager, new_map_manager_can_register_types)
{
    auto map_manager = std::make_shared<Gecko::MapManager>();
    auto map_function = static_cast<Gecko::Map*(*)(Gecko::Map* memory, const std::shared_ptr<Gecko::Configuration>&)>(&Gecko::Map::create);
    auto map_factory = std::bind(map_function, std::placeholders::_1, std::make_shared<Gecko::Configuration>());

    map_manager->register_type<Gecko::Map>("map", map_factory);

    EXPECT_EQ(map_manager->get_size("map"), 0);
    EXPECT_EQ(map_manager->get_max_size("map"), 0);
}

TEST(map_manager, new_map_manager_can_allocate_memory)
{
    auto map_manager = std::make_shared<Gecko::MapManager>();
    auto map_configuration = std::make_shared<Gecko::Configuration>("../maps/tutorial/tutorial.json");
    auto map_function = static_cast<Gecko::Map*(*)(Gecko::Map* memory, const std::shared_ptr<Gecko::Configuration>&)>(&Gecko::Map::create);
    auto map_factory = std::bind(map_function, std::placeholders::_1, map_configuration);

    map_manager->register_type<Gecko::Map>("map", map_factory);
    map_manager->allocate("map", 4);

    EXPECT_EQ(map_manager->get_size("map"), 0);
    EXPECT_EQ(map_manager->get_max_size("map"), 4);
}
