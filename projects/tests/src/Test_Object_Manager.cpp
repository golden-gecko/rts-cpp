#include <gtest/gtest.h>

#include <Gecko/Configuration.hpp>
#include <Gecko/Managers/ObjectManager.hpp>
#include <Gecko/Objects/Vehicle.hpp>

#include "Fixtures.hpp"

/*
TEST(object_manager, new_object_manager_is_empty)
{
    auto configuration = std::make_shared<Gecko::Configuration>();
    auto object_manager = std::make_shared<Gecko::ObjectManager>();

    EXPECT_EQ(object_manager->is_type_registered("truck"), false);
}

TEST(object_manager, new_object_manager_can_register_types)
{
    auto object_manager = std::make_shared<Gecko::ObjectManager>();
    auto object_configuration = std::make_shared<Gecko::Configuration>("../objects/vehicles/civilian/truck.json");
    auto object_function = static_cast<Gecko::Vehicle*(*)(Gecko::Vehicle* memory, const std::shared_ptr<Gecko::Configuration>&)>(&Gecko::Vehicle::create);
    auto object_factory = std::bind(object_function, std::placeholders::_1, object_configuration);

    object_manager->register_type<Gecko::Vehicle>("truck", object_factory);

    EXPECT_EQ(object_manager->get_size("truck"), 0);
    EXPECT_EQ(object_manager->get_max_size("truck"), 0);
}

TEST(object_manager, new_object_manager_can_allocate_memory)
{
    init_game init_game;

    // TODO: Decouple.
    Gecko::ObjectManager::getSingletonPtr()->unregister_all();

    auto object_manager = Gecko::ObjectManager::getSingletonPtr();
    auto object_configuration = std::make_shared<Gecko::Configuration>("../objects/vehicles/civilian/truck.json");
    auto object_function = static_cast<Gecko::Vehicle*(*)(Gecko::Vehicle* memory, const std::shared_ptr<Gecko::Configuration>&)>(&Gecko::Vehicle::create);
    auto object_factory = std::bind(object_function, std::placeholders::_1, object_configuration);

    object_manager->register_type<Gecko::Vehicle>("truck", object_factory);
    object_manager->allocate("truck", 8);

    EXPECT_EQ(object_manager->get_size("truck"), 0);
    EXPECT_EQ(object_manager->get_max_size("truck"), 8);
}
*/
