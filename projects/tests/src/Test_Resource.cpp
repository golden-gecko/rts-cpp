#include <gtest/gtest.h>

#include <Gecko/Configuration.hpp>
#include <Gecko/Resource.hpp>

TEST(resource, new_resource_is_empty)
{
    Gecko::Resource resource("");

    EXPECT_EQ(resource.get_name(), "");
    EXPECT_FLOAT_EQ(resource.get_current(), 0.0f);
    EXPECT_FLOAT_EQ(resource.get_max(), 0.0f);
}

TEST(resource, cannot_add_or_remove_from_empty_resource)
{
    Gecko::Resource resource("");

    EXPECT_FLOAT_EQ(resource.add(1.0f), 0.0f);
    EXPECT_FLOAT_EQ(resource.remove(1.0f), 0.0f);
}

TEST(resource, can_add_or_remove_from_resource)
{
    Gecko::ConfigurationPtr configuration = std::make_shared<Gecko::Configuration>("../tests/resources/resource.json");
    Gecko::ResourcePtr resource = std::make_shared<Gecko::Resource>("Coal");

    resource->deserialize(configuration);

    EXPECT_FLOAT_EQ(resource->add(20.0f), 20.0f);
    EXPECT_FLOAT_EQ(resource->remove(10.0f), 10.0f);
}
