#include <gtest/gtest.h>

#include <Gecko/Configuration.hpp>
#include <Gecko/Containers/Resources.hpp>

TEST(resources, new_resource_container_is_empty)
{
    Gecko::Resources resources;

    EXPECT_EQ(resources.has_resource("Iron Ore"), false);
    EXPECT_EQ(resources.has_storage("Iron Ore"), false);

    EXPECT_FLOAT_EQ(resources.get_current("Iron Ore"), 0.0f);
    EXPECT_FLOAT_EQ(resources.get_storage("Iron Ore"), 0.0f);
}

TEST(resources, cannot_add_or_remove_from_empty_resource_container)
{
    Gecko::Resources resources;

    EXPECT_FLOAT_EQ(resources.add("Iron Ore", 1.0f), 0.0f);
    EXPECT_FLOAT_EQ(resources.remove("Iron Ore", 1.0f), 0.0f);
}

TEST(resources, cannot_add_or_remove_inexistent_item_from_container)
{
    Gecko::ConfigurationPtr configuration = std::make_shared<Gecko::Configuration>("../tests/resources/storage.json");
    Gecko::ResourcesPtr resources = std::make_shared<Gecko::Resources>();

    resources->deserialize(configuration);

    EXPECT_EQ(resources->has_resource("Energy"), false);
    EXPECT_EQ(resources->has_storage("Energy"), false);

    EXPECT_FLOAT_EQ(resources->add("Energy", 40.0f), 0.0f);
    EXPECT_FLOAT_EQ(resources->remove("Energy", 20.0f), 0.0f);
}

TEST(resources, can_add_or_remove_from_resource_container)
{
    Gecko::ConfigurationPtr configuration = std::make_shared<Gecko::Configuration>("../tests/resources/storage.json");
    Gecko::ResourcesPtr resources = std::make_shared<Gecko::Resources>();

    resources->deserialize(configuration);

    EXPECT_EQ(resources->has_resource("Iron Ore"), true);
    EXPECT_EQ(resources->has_storage("Iron Ore"), true);

    EXPECT_FLOAT_EQ(resources->add("Iron Ore", 20.0f), 20.0f);
    EXPECT_FLOAT_EQ(resources->remove("Iron Ore", 10.0f), 10.0f);
}

TEST(resources, can_merge_resource_containers)
{
    Gecko::ResourcesPtr factory = std::make_shared<Gecko::Resources>();
    Gecko::ResourcesPtr ironworks = std::make_shared<Gecko::Resources>();

    factory->deserialize(std::make_shared<Gecko::Configuration>("../tests/resources/merge/factory.json"));
    ironworks->deserialize(std::make_shared<Gecko::Configuration>("../tests/resources/merge/ironworks.json"));

    EXPECT_EQ(factory->has_resource("Coal"), true);
    EXPECT_EQ(factory->has_resource("Iron Ore"), true);
    EXPECT_EQ(factory->has_resource("Unprocessed Iron Ore"), false);

    factory->merge(*(ironworks.get()));

    EXPECT_EQ(factory->has_resource("Coal"), true);
    EXPECT_EQ(factory->has_resource("Iron Ore"), true);
    EXPECT_EQ(factory->has_resource("Unprocessed Iron Ore"), true);

    EXPECT_FLOAT_EQ(factory->get_current("Coal"), 20.0f);
    EXPECT_FLOAT_EQ(factory->get_current("Iron Ore"), 60.0f);
    EXPECT_FLOAT_EQ(factory->get_current("Unprocessed Iron Ore"), 200.0f);

    EXPECT_FLOAT_EQ(factory->get_max("Coal"), 80.0f);
    EXPECT_FLOAT_EQ(factory->get_max("Iron Ore"), 120.0f);
    EXPECT_FLOAT_EQ(factory->get_max("Unprocessed Iron Ore"), 800.0f);

}
