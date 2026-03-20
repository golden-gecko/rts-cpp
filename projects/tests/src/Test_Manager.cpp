#include <gtest/gtest.h>

#include <Gecko/Log.hpp>
#include <Gecko/Managers/Manager.hpp>

namespace
{
    class Object
    {
    public:
        Object()
        {
            L_TRACE << "Object::Object()";
        }

        Object(const Object& other)
        {
            L_TRACE << "Object::Object(const Object& other)";
        }

        Object(Object&& other)
        {
            L_TRACE << "Object::Object(Object&& other)";
        }

        virtual ~Object()
        {
            L_TRACE << "Object::~Object()";
        }

        virtual void init()
        {
        }

        virtual void deinit()
        {
        }

        virtual void update(float time)
        {
        }

    public:
        std::uint16_t get_id() const
        {
            return id;
        }

        virtual std::string get_name() const = 0;

    public:
        void set_id(const std::uint16_t& _id)
        {
            id = _id;
        }

    private:
        std::uint16_t id = 0;
    };

    class Car :
        public Object
    {
    private:
        using base_type = Object;

    public:
        static Car* create(Car* memory)
        {
            return new (memory) Car();
        }

    public:
        Car()
        {
            L_TRACE << "Car::Car()";
        }

        Car(const Car& other) :
            base_type(other)
        {
            L_TRACE << "Car::Car(const Car& other)";
        }

        Car(Car&& other) :
            base_type(other)
        {
            L_TRACE << "Car::Car(Car&& other)";
        }

        ~Car() override
        {
            L_TRACE << "Car::~Car()";
        }

    public:
        std::string get_name() const override
        {
            return "Car Name";
        }
    };

    class Tank :
        public Object
    {
    private:
        using base_type = Object;

    public:
        static Tank* create(Tank* memory)
        {
            return new (memory) Tank();
        }

    public:
        Tank()
        {
            L_TRACE << "Tank::Tank(const Car& other)";
        }

        Tank(const Tank& other) :
            base_type(other)
        {
            L_TRACE << "Tank::Tank(const Tank& other)";
        }

        Tank(Tank&& other) :
            base_type(other)
        {
            L_TRACE << "Tank::Tank(Tank&& other)";
        }

        ~Tank() override
        {
            L_TRACE << "Tank::~Tank()";
        }

    public:
        std::string get_name() const override
        {
            return "Tank Name";
        }
    };
}

TEST(manager, not_registered_objects_can_not_be_allocated_or_created)
{
    auto manager = Gecko::Manager<Object, std::string, std::uint16_t>();

    EXPECT_EQ(manager.is_type_registered("Car"), false);
    EXPECT_EQ(manager.allocate("Car", 2), false);
    EXPECT_EQ(manager.create("Car"), nullptr);
    EXPECT_EQ(manager.at("Car", 10), nullptr);
}

TEST(manager, registered_but_not_allocated_objects_can_not_be_created)
{
    auto manager = Gecko::Manager<Object, std::string, std::uint16_t>();

    EXPECT_EQ(manager.register_type<Car>("Car", std::bind(Car::create, std::placeholders::_1)), true);
    EXPECT_EQ(manager.is_type_registered("Car"), true);
    EXPECT_EQ(manager.create("Car"), nullptr);
    EXPECT_EQ(manager.at("Car", 0), nullptr);
}

TEST(manager, registered_and_allocated_objects_can_be_created)
{
    auto manager = Gecko::Manager<Object, std::string, std::uint16_t>();

    EXPECT_EQ(manager.register_type<Car>("Car", std::bind(Car::create, std::placeholders::_1)), true);
    EXPECT_EQ(manager.is_type_registered("Car"), true);
    EXPECT_EQ(manager.allocate("Car", 2), true);
    EXPECT_EQ(manager.get_size("Car"), 0);
    EXPECT_EQ(manager.get_max_size("Car"), 2);

    EXPECT_NE(manager.create("Car"), nullptr);

    EXPECT_EQ(manager.get_size("Car"), 1);
    EXPECT_EQ(manager.get_max_size("Car"), 2);

    EXPECT_NE(manager.create("Car"), nullptr);

    EXPECT_EQ(manager.get_size("Car"), 2);
    EXPECT_EQ(manager.get_max_size("Car"), 2);

    EXPECT_EQ(manager.create("Car"), nullptr);

    EXPECT_NE(manager.at("Car", 0), nullptr);
    EXPECT_NE(manager.at("Car", 1), nullptr);
    EXPECT_EQ(manager.at("Car", 2), nullptr);
}

TEST(manager, registered_and_allocated_objects_can_be_created_and_destroyed)
{
    auto manager = Gecko::Manager<Object, std::string, std::uint16_t>();

    EXPECT_EQ(manager.register_type<Car>("Car", std::bind(Car::create, std::placeholders::_1)), true);
    EXPECT_EQ(manager.is_type_registered("Car"), true);
    EXPECT_EQ(manager.allocate("Car", 4), true);

    EXPECT_NE(manager.create("Car"), nullptr);
    EXPECT_NE(manager.create("Car"), nullptr);
    EXPECT_NE(manager.create("Car"), nullptr);
    EXPECT_NE(manager.create("Car"), nullptr);

    EXPECT_EQ(manager.get_size("Car"), 4);
    EXPECT_EQ(manager.get_max_size("Car"), 4);

    manager.destroy(1);
    manager.destroy(3);

    EXPECT_EQ(manager.get_size("Car"), 2);
    EXPECT_EQ(manager.get_max_size("Car"), 4);

    EXPECT_NE(manager.create("Car"), nullptr);
    EXPECT_NE(manager.create("Car"), nullptr);

    EXPECT_EQ(manager.get_size("Car"), 4);
    EXPECT_EQ(manager.get_max_size("Car"), 4);

    EXPECT_EQ(manager.at("Car", 0)->get_id(), 4);
    EXPECT_EQ(manager.at("Car", 1)->get_id(), 2);
    EXPECT_EQ(manager.at("Car", 2)->get_id(), 5);
    EXPECT_EQ(manager.at("Car", 3)->get_id(), 6);

    EXPECT_EQ(manager.at("Car", 0)->get_name(), "Car Name");
    EXPECT_EQ(manager.at("Car", 1)->get_name(), "Car Name");
    EXPECT_EQ(manager.at("Car", 2)->get_name(), "Car Name");
    EXPECT_EQ(manager.at("Car", 3)->get_name(), "Car Name");
}

TEST(manager, created_objects_has_correct_ids)
{
    auto manager = Gecko::Manager<Object, std::string, std::uint16_t>();

    EXPECT_EQ(manager.register_type<Car>("Car", std::bind(Car::create, std::placeholders::_1)), true);
    EXPECT_EQ(manager.register_type<Tank>("Tank", std::bind(Tank::create, std::placeholders::_1)), true);

    EXPECT_EQ(manager.allocate("Car", 6), true);
    EXPECT_EQ(manager.allocate("Tank", 2), true);

    EXPECT_EQ(manager.get_size("Car"), 0);
    EXPECT_EQ(manager.get_size("Tank"), 0);

    EXPECT_EQ(manager.get_max_size("Car"), 6);
    EXPECT_EQ(manager.get_max_size("Tank"), 2);

    EXPECT_NE(manager.create("Car"), nullptr);
    EXPECT_NE(manager.create("Car"), nullptr);

    EXPECT_NE(manager.create("Tank"), nullptr);
    EXPECT_NE(manager.create("Tank"), nullptr);

    EXPECT_NE(manager.create("Car"), nullptr);
    EXPECT_NE(manager.create("Car"), nullptr);

    EXPECT_EQ(manager.get_size("Car"), 4);
    EXPECT_EQ(manager.get_size("Tank"), 2);

    EXPECT_EQ(manager.get_max_size("Car"), 6);
    EXPECT_EQ(manager.get_max_size("Tank"), 2);

    EXPECT_EQ(manager.at("Car", 0)->get_id(), 1);
    EXPECT_EQ(manager.at("Car", 1)->get_id(), 2);
    EXPECT_EQ(manager.at("Car", 2)->get_id(), 5);
    EXPECT_EQ(manager.at("Car", 3)->get_id(), 6);

    EXPECT_EQ(manager.at("Car", 0)->get_name(), "Car Name");
    EXPECT_EQ(manager.at("Car", 1)->get_name(), "Car Name");
    EXPECT_EQ(manager.at("Car", 2)->get_name(), "Car Name");
    EXPECT_EQ(manager.at("Car", 3)->get_name(), "Car Name");

    EXPECT_EQ(manager.at("Tank", 0)->get_id(), 3);
    EXPECT_EQ(manager.at("Tank", 1)->get_id(), 4);

    EXPECT_EQ(manager.at("Tank", 0)->get_name(), "Tank Name");
    EXPECT_EQ(manager.at("Tank", 1)->get_name(), "Tank Name");
}

TEST(manager, created_and_destroyed_objects_has_correct_id)
{
    auto manager = Gecko::Manager<Object, std::string, std::uint16_t>();

    EXPECT_EQ(manager.register_type<Car>("Car", std::bind(Car::create, std::placeholders::_1)), true);
    EXPECT_EQ(manager.register_type<Tank>("Tank", std::bind(Tank::create, std::placeholders::_1)), true);

    EXPECT_EQ(manager.allocate("Car", 8), true);
    EXPECT_EQ(manager.allocate("Tank", 4), true);

    EXPECT_NE(manager.create("Car"), nullptr);
    EXPECT_NE(manager.create("Car"), nullptr);
    EXPECT_NE(manager.create("Car"), nullptr);
    EXPECT_NE(manager.create("Car"), nullptr);

    manager.destroy(1);
    manager.destroy(3);

    EXPECT_NE(manager.create("Tank"), nullptr);
    EXPECT_NE(manager.create("Tank"), nullptr);

    EXPECT_EQ(manager.get(2)->get_id(), 2);
    EXPECT_EQ(manager.get(4)->get_id(), 4);
    EXPECT_EQ(manager.get(5)->get_id(), 5);
    EXPECT_EQ(manager.get(6)->get_id(), 6);

    EXPECT_EQ(manager.get(2)->get_name(), "Car Name");
    EXPECT_EQ(manager.get(4)->get_name(), "Car Name");

    EXPECT_EQ(manager.get(5)->get_name(), "Tank Name");
    EXPECT_EQ(manager.get(6)->get_name(), "Tank Name");
}

TEST(manager, all_created_objects_can_be_destroyed_at_once)
{
    auto manager = Gecko::Manager<Object, std::string, std::uint16_t>();

    EXPECT_EQ(manager.register_type<Car>("Car", std::bind(Car::create, std::placeholders::_1)), true);
    EXPECT_EQ(manager.register_type<Tank>("Tank", std::bind(Tank::create, std::placeholders::_1)), true);

    EXPECT_EQ(manager.allocate("Car", 4), true);
    EXPECT_EQ(manager.allocate("Tank", 2), true);

    EXPECT_NE(manager.create("Car"), nullptr);
    EXPECT_NE(manager.create("Car"), nullptr);
    EXPECT_NE(manager.create("Car"), nullptr);
    EXPECT_NE(manager.create("Car"), nullptr);

    EXPECT_NE(manager.create("Tank"), nullptr);
    EXPECT_NE(manager.create("Tank"), nullptr);

    EXPECT_EQ(manager.size(), 6);

    manager.destroy_all();

    EXPECT_EQ(manager.size(), 0);
}
