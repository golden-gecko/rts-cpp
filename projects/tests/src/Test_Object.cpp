#include <gtest/gtest.h>

#include <Gecko/Configuration.hpp>
#include <Gecko/Containers/Components.hpp>
#include <Gecko/Objects/Object.hpp>
#include <Gecko/Scenes/Scene.hpp>

#include "Fixtures.hpp"

TEST(object, new_object_is_empty)
{
    auto scene_configuration = std::make_shared<Gecko::Configuration>();
    auto scene = std::make_shared<Gecko::Scene>(scene_configuration);
    auto object = std::make_shared<Gecko::Object>(scene);

    EXPECT_EQ(object->get_configuration().get(), nullptr);
    EXPECT_EQ(object->get_id(), Gecko::Id::Empty);
    EXPECT_EQ(object->get_name(), "");
    EXPECT_EQ(object->get_components()->size(), 0);
}

TEST(object, object_deserialized_from_configuration_is_loaded)
{
    init_game init_game;

    auto scene_configuration = std::make_shared<Gecko::Configuration>();
    auto scene = std::make_shared<Gecko::Scene>(scene_configuration);
    auto object_configuration = std::make_shared<Gecko::Configuration>("../tests/vehicles/tank.json");
    auto object = std::make_shared<Gecko::Object>(scene);

    object->deserialize(object_configuration);

    EXPECT_EQ(object->get_configuration()->get_name(), "tank");
    EXPECT_EQ(object->get_id(), Gecko::Id::Empty);
    EXPECT_EQ(object->get_name(), "Tank");
    EXPECT_EQ(object->get_components()->size(), 5);
}

TEST(object, object_deserialized_and_initialized_from_configuration_is_loaded)
{
    init_game init_game;

    auto scene_configuration = std::make_shared<Gecko::Configuration>();
    auto scene = std::make_shared<Gecko::Scene>(scene_configuration);
    auto object_configuration = std::make_shared<Gecko::Configuration>("../tests/vehicles/tank.json");
    auto object = std::make_shared<Gecko::Object>(scene);

    object->deserialize(object_configuration);
    object->init();

    EXPECT_EQ(object->get_configuration()->get_name(), "tank");
    EXPECT_EQ(object->get_id(), Gecko::Id::Empty);
    EXPECT_EQ(object->get_name(), "Tank");
    EXPECT_EQ(object->get_components()->size(), 5);
}
