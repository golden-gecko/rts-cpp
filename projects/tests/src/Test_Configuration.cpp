#include <gtest/gtest.h>

#include "Configuration.hpp"

namespace
{
    bool has_members(const Gecko::Configuration& configuration)
    {
        for (const auto& i : configuration)
        {
            return true;
        }

        return false;
    }
}

TEST(configuration, new_configuration_is_empty)
{
    Gecko::Configuration configuration;

    EXPECT_EQ(configuration.size(), 0);
}

TEST(configuration, new_configuration_has_no_members)
{
    Gecko::Configuration configuration;

    EXPECT_EQ(has_members(configuration), false);
}

TEST(configuration, configuration_from_file_has_members)
{
    Gecko::Configuration configuration(std::string("../games/rts.json"));

    EXPECT_EQ(has_members(configuration), true);
}

TEST(configuration, can_retrieve_single_keys_from_configuration)
{
    Gecko::Configuration configuration("../games/rts.json");

    EXPECT_EQ(configuration.get_string("name"), "RTS");
}

TEST(configuration, can_retrieve_paths_from_configuration)
{
    Gecko::Configuration configuration("../games/rts.json");

    EXPECT_EQ(configuration.get_string("ui.font"), "Arial");
}

TEST(configuration, bool_value_can_be_saved_and_retrieved_from_single_key)
{
    auto configuration = std::make_shared<Gecko::Configuration>();

    configuration->set("active", false);

    EXPECT_EQ(configuration->get_bool("active"), false);
}

TEST(configuration, float_value_can_be_saved_and_retrieved_from_single_key)
{
    auto configuration = std::make_shared<Gecko::Configuration>();

    configuration->set("damage", 20.0f);

    EXPECT_EQ(configuration->get_float("damage"), 20.0f);
}

TEST(configuration, string_value_can_be_saved_and_retrieved_from_single_key)
{
    auto configuration = std::make_shared<Gecko::Configuration>();

    configuration->set("name", "Factory");

    EXPECT_EQ(configuration->get_string("name"), "Factory");
}

TEST(configuration, values_can_be_saved_and_retrieved_from_paths)
{
    auto configuration = std::make_shared<Gecko::Configuration>();

    configuration->set("components.weapon.name", "Cannon");

    EXPECT_EQ(configuration->get_string("components.weapon.name"), "Cannon");
}

TEST(configuration, child_configuration_is_not_removed_when_parent_is_removed)
{
    std::shared_ptr<Gecko::Configuration> cameras;

    EXPECT_EQ(cameras.get(), nullptr);

    {
        auto configuration = std::make_shared<Gecko::Configuration>("../games/rts.json");

        cameras = configuration->get_child("cameras");
    }

    EXPECT_NE(cameras.get(), nullptr);
}

TEST(configuration, can_get_configuration_size)
{
    Gecko::Configuration configuration("../tests/vehicles/tank.json");

    EXPECT_EQ(configuration.size(), 3);
}

TEST(configuration, can_iterate_over_members)
{
    std::vector<std::string> members;

    Gecko::Configuration configuration("../tests/vehicles/tank.json");

    for (auto i = configuration.begin(); i != configuration.end(); ++i)
    {
        members.emplace_back(i.key().asString());
    }

    EXPECT_EQ(members[0], "components");
    EXPECT_EQ(members[1], "creatable");
    EXPECT_EQ(members[2], "name");
}

TEST(configuration, append_to_array)
{
    Gecko::Configuration configuration;

    configuration.append("numbers", 0);
    configuration.append("numbers", 1);
    configuration.append("numbers", 2);

    EXPECT_EQ(configuration.get_child("numbers")->size(), 3);
}
