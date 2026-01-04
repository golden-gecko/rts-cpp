#include <gtest/gtest.h>

#include <Gecko/Configuration.hpp>
#include <Gecko/Containers/Components.hpp>
#include <Gecko/Objects/Object.hpp>

#include "Fixtures.hpp"

TEST(object, new_object_is_empty)
{
    auto object = std::make_shared<Gecko::Object>();

    EXPECT_EQ(object->get_configuration().get(), nullptr);
    EXPECT_EQ(object->get_id(), 0);
    EXPECT_EQ(object->get_name(), "");
    EXPECT_EQ(object->get_components()->size(), 0);
}

TEST(object, object_deserialized_from_configuration_is_loaded)
{
    init_game init_game({});

    auto object_configuration = std::make_shared<Gecko::Configuration>("../objects/plants/tree.json");
    auto object = std::make_shared<Gecko::Object>();

    object->deserialize(object_configuration);

    EXPECT_EQ(object->get_configuration()->get_name(), "tree");
    EXPECT_EQ(object->get_id(), 0);
    EXPECT_EQ(object->get_name(), "Tree");
    EXPECT_EQ(object->get_components()->size(), 1);
}

TEST(object, object_deserialized_and_initialized_from_configuration_is_loaded)
{
    init_game init_game({});

    auto object_configuration = std::make_shared<Gecko::Configuration>("../objects/plants/tree.json");
    auto object = std::make_shared<Gecko::Object>();

    object->deserialize(object_configuration);
    object->init();

    EXPECT_EQ(object->get_configuration()->get_name(), "tree");
    EXPECT_EQ(object->get_id(), 0);
    EXPECT_EQ(object->get_name(), "Tree");
    EXPECT_EQ(object->get_components()->size(), 1);
}
