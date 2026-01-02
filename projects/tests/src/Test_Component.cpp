#include <gtest/gtest.h>

#include <Gecko/Components/Component.hpp>

TEST(component, new_component_is_empty)
{
    Gecko::Component component;

    EXPECT_EQ(component.get_configuration().get(), nullptr);
    EXPECT_EQ(component.get_id(), 0);
    EXPECT_EQ(component.get_name(), "");
}
