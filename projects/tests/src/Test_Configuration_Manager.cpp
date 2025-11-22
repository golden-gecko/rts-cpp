#include <gtest/gtest.h>

#include "Exception.hpp"
#include "Managers/ConfigurationManager.hpp"

TEST(configuration_manager, new_configuration_is_empty)
{
    Gecko::ConfigurationManager manager;

    EXPECT_EQ(manager.get_configurations().size(), 0);
}

TEST(configuration_manager, can_not_load_multiple_configurations_with_the_same_name)
{
    Gecko::ConfigurationManager manager;

    EXPECT_NO_THROW(manager.parse_directory("../tests/units"));
    EXPECT_THROW(manager.parse_directory("../tests/vehicles"), Gecko::Exception);
}
