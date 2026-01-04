#include <gtest/gtest.h>

#include <Gecko/Configuration.hpp>
#include <Gecko/Maps/Map.hpp>

#include "Fixtures.hpp"

TEST(map, new_map_is_empty)
{
    auto map = std::make_shared<Gecko::Map>();

    EXPECT_EQ(map->get_configuration().get(), nullptr);
    EXPECT_EQ(map->get_id(), 0);
    EXPECT_EQ(map->get_name(), "");
    EXPECT_EQ(map->get_layers().size(), 0);
    EXPECT_EQ(map->get_seasons().size(), 0);
}

TEST(map, map_deserialized_from_configuration_is_loaded)
{
    init_game init_game({});

    auto map_configuration = std::make_shared<Gecko::Configuration>("../maps/tutorial/tutorial.json");
    auto map = std::make_shared<Gecko::Map>();

    map->deserialize(map_configuration);

    EXPECT_EQ(map->get_configuration()->get_name(), "tutorial");
    EXPECT_EQ(map->get_id(), 0);
    EXPECT_EQ(map->get_name(), "Tutorial");
    EXPECT_EQ(map->get_layers().size(), 0);
    EXPECT_EQ(map->get_seasons().size(), 0);
}

TEST(map, map_deserialized_and_initialized_from_configuration_is_loaded)
{
    init_game init_game({});

    auto map_configuration = std::make_shared<Gecko::Configuration>("../maps/tutorial/tutorial.json");
    auto map = std::make_shared<Gecko::Map>();

    map->deserialize(map_configuration);
    map->init();

    EXPECT_EQ(map->get_configuration()->get_name(), "tutorial");
    EXPECT_EQ(map->get_id(), 0);
    EXPECT_EQ(map->get_name(), "Tutorial");
    EXPECT_EQ(map->get_layers().size(), 1);
    EXPECT_EQ(map->get_seasons().size(), 0);
}
