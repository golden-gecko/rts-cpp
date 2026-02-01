#include <gtest/gtest.h>

#include <Gecko/Configuration.hpp>

TEST(configuration, new_configuration_is_empty)
{
    Gecko::Configuration configuration;

    EXPECT_EQ(configuration.get_file_name(), "");
    EXPECT_EQ(configuration.get_name(), "");
    EXPECT_EQ(configuration.size(), 0);
}

TEST(configuration, configuration_from_file_has_members)
{
    Gecko::Configuration configuration("../tests/units/tank.json");

    EXPECT_EQ(configuration.get_file_name(), "../tests/units/tank.json");
    EXPECT_EQ(configuration.get_name(), "tank");
    EXPECT_EQ(configuration.size(), 3);
}

TEST(configuration, can_retrieve_single_keys_from_configuration)
{
    Gecko::Configuration configuration("../tests/units/tank.json");

    EXPECT_EQ(configuration.get_string("name"), "Tank");
}

TEST(configuration, can_retrieve_paths_from_configuration)
{
    Gecko::Configuration configuration("../tests/units/tank.json");

    EXPECT_EQ(configuration.get_string("path.with.nested.value"), "OK");
}

TEST(configuration, bool_value_can_be_saved_and_retrieved_from_single_key)
{
    Gecko::Configuration configuration;

    configuration.set("active", false);

    EXPECT_EQ(configuration.get_bool("active"), false);
}

TEST(configuration, float_value_can_be_saved_and_retrieved_from_single_key)
{
    Gecko::Configuration configuration;

    configuration.set("damage", 20.0f);

    EXPECT_EQ(configuration.get_float("damage"), 20.0f);
}

TEST(configuration, string_value_can_be_saved_and_retrieved_from_single_key)
{
    Gecko::Configuration configuration;

    configuration.set("name", "Factory");

    EXPECT_EQ(configuration.get_string("name"), "Factory");
}

TEST(configuration, values_can_be_saved_and_retrieved_from_paths)
{
    Gecko::Configuration configuration;

    configuration.set("components.weapon.name", "Cannon");

    EXPECT_EQ(configuration.get_string("components.weapon.name"), "Cannon");
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

TEST(configuration, set_path)
{
    Gecko::Navigation::Path path;

    path.set_points({
        Gecko::Navigation::Coordinate(1, 2),
        Gecko::Navigation::Coordinate(2, 4),
        Gecko::Navigation::Coordinate(3, 6),
    });

    Gecko::Configuration configuration;

    configuration.set("path", path);

    EXPECT_EQ(configuration.get_child("path")->size(), 3);
    EXPECT_EQ(configuration.get_child("path")->get_element(0)->get_int("x"), 1);
    EXPECT_EQ(configuration.get_child("path")->get_element(1)->get_int("x"), 2);
    EXPECT_EQ(configuration.get_child("path")->get_element(2)->get_int("x"), 3);
}

TEST(configuration, get_path)
{
    Gecko::Configuration configuration("../tests/configurations/navigation.json");

    Gecko::Navigation::Path path = configuration.get_path("target");

    EXPECT_EQ(path.get_points().at(0).x, 10);
    EXPECT_EQ(path.get_points().at(1).x, 14);
    EXPECT_EQ(path.get_points().at(2).x, 18);
}

TEST(configuration, is_include_merging)
{
    Gecko::Configuration configuration("../tests/include/tree.json");

    EXPECT_EQ(configuration.get_string("unique_property_from_plant"), "plant name");
    EXPECT_EQ(configuration.get_string("unique_property_from_tree"), "tree name");
}

TEST(configuration, is_include_overwriting)
{
    Gecko::Configuration plant("../tests/include/plant.json");
    Gecko::Configuration tree("../tests/include/tree.json");

    EXPECT_EQ(plant.get_string("property_from_plant"), "this is plant");
    EXPECT_EQ(tree.get_string("property_from_plant"), "this is tree");
}
