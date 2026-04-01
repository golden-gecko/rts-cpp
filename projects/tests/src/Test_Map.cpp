#include <gtest/gtest.h>

#include <Gecko/Configuration.hpp>
#include <Gecko/Maps/Map.hpp>
#include <Gecko/Scenes/Scene.hpp>

#include "Fixtures.hpp"

TEST(map, new_map_is_empty)
{
    init_game init_game;

    auto game_configuration = std::make_shared<Gecko::Configuration>("../games/rts.json");
    auto map_configuration = std::make_shared<Gecko::Configuration>("../maps/tutorial/tutorial.json");
    auto scene_configuration = game_configuration->get_child("scenes.Map");

    auto scene = std::make_shared<Gecko::Scene>(scene_configuration);
    auto map = std::make_shared<Gecko::Map>(scene);

    EXPECT_EQ(map->get_configuration().get(), nullptr);
    EXPECT_EQ(map->get_id(), Gecko::Id::Empty);
    EXPECT_EQ(map->get_name(), "");
    EXPECT_EQ(map->get_layers().size(), 0);
    EXPECT_EQ(map->get_seasons().size(), 0);
}

TEST(map, map_deserialized_from_configuration_is_loaded)
{
    init_game init_game;

    auto game_configuration = std::make_shared<Gecko::Configuration>("../games/rts.json");
    auto map_configuration = std::make_shared<Gecko::Configuration>("../maps/tutorial/tutorial.json");
    auto scene_configuration = game_configuration->get_child("scenes.Map");

    auto scene = std::make_shared<Gecko::Scene>(scene_configuration);
    auto map = std::make_shared<Gecko::Map>(scene);

    map->deserialize(map_configuration);

    EXPECT_EQ(map->get_configuration()->get_name(), "tutorial");
    EXPECT_EQ(map->get_id(), Gecko::Id::Empty);
    EXPECT_EQ(map->get_name(), "Tutorial");
    EXPECT_EQ(map->get_layers().size(), 0);
    EXPECT_EQ(map->get_seasons().size(), 0);
}

TEST(map, map_deserialized_and_initialized_from_configuration_is_loaded)
{
    init_game init_game;

    auto game_configuration = std::make_shared<Gecko::Configuration>("../games/rts.json");
    auto map_configuration = std::make_shared<Gecko::Configuration>("../maps/tutorial/tutorial.json");
    auto scene_configuration = game_configuration->get_child("scenes.Map");

    auto scene = std::make_shared<Gecko::Scene>(scene_configuration);
    auto map = std::make_shared<Gecko::Map>(scene);

    map->deserialize(map_configuration);
    map->init();

    EXPECT_EQ(map->get_configuration()->get_name(), "tutorial");
    EXPECT_EQ(map->get_id(), Gecko::Id::Empty);
    EXPECT_EQ(map->get_name(), "Tutorial");
    EXPECT_EQ(map->get_layers().size(), 1);
    EXPECT_EQ(map->get_seasons().size(), 0);
}
