#include <gtest/gtest.h>

#include <Gecko/Managers/Manager.hpp>

namespace
{
    class Object
    {
    public:
        Object()
        {
        }

        Object(const Object& other)
        {
        }

        virtual ~Object()
        {
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
        }

        Car(const Car& other) :
            base_type(other)
        {
        }

        ~Car() override
        {
        }

    public:
        std::string get_name() const override
        {
            return "Car";
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
        }

        Tank(const Tank& other) :
            base_type(other)
        {
        }

        ~Tank() override
        {
        }

    public:
        std::string get_name() const override
        {
            return "Tank";
        }
    };
}

/*
TEST(manager, not_registered_objects_can_not_be_allocated_or_created)
{
    auto manager = Gecko::Manager<Object, std::string, std::uint16_t>();

    EXPECT_EQ(manager.is_type_registered("car"), false);
    EXPECT_EQ(manager.allocate("car", 2), false);
    EXPECT_EQ(manager.create("car"), nullptr);
    EXPECT_EQ(manager.at("car", 10), nullptr);
}

TEST(manager, registered_but_not_allocated_objects_can_not_be_created)
{
    auto manager = Gecko::Manager<Object, std::string, std::uint16_t>();
    auto car_factory = std::bind(Car::create, std::placeholders::_1);

    EXPECT_EQ(manager.register_type<Car>("car", car_factory), true);
    EXPECT_EQ(manager.is_type_registered("car"), true);
    EXPECT_EQ(manager.create("car"), nullptr);
    EXPECT_EQ(manager.at("car", 0), nullptr);
}

TEST(manager, registered_and_allocated_objects_can_be_created)
{
    auto manager = Gecko::Manager<Object, std::string, std::uint16_t>();
    auto car_factory = std::bind(Car::create, std::placeholders::_1);

    EXPECT_EQ(manager.register_type<Car>("car", car_factory), true);
    EXPECT_EQ(manager.is_type_registered("car"), true);
    EXPECT_EQ(manager.allocate("car", 2), true);
    EXPECT_EQ(manager.get_size("car"), 0);
    EXPECT_EQ(manager.get_max_size("car"), 2);

    EXPECT_NE(manager.create("car"), nullptr);

    EXPECT_EQ(manager.get_size("car"), 1);
    EXPECT_EQ(manager.get_max_size("car"), 2);

    EXPECT_NE(manager.create("car"), nullptr);

    EXPECT_EQ(manager.get_size("car"), 2);
    EXPECT_EQ(manager.get_max_size("car"), 2);

    EXPECT_EQ(manager.create("car"), nullptr);

    EXPECT_NE(manager.at("car", 0), nullptr);
    EXPECT_NE(manager.at("car", 1), nullptr);

    EXPECT_EQ(manager.at("car", 2), nullptr);
}

TEST(manager, registered_and_allocated_objects_can_be_created_and_destroyed)
{
    auto manager = Gecko::Manager<Object, std::string, std::uint16_t>();
    auto car_factory = std::bind(Car::create, std::placeholders::_1);

    EXPECT_EQ(manager.register_type<Car>("car", car_factory), true);
    EXPECT_EQ(manager.is_type_registered("car"), true);
    EXPECT_EQ(manager.allocate("car", 4), true);

    EXPECT_NE(manager.create("car"), nullptr);
    EXPECT_NE(manager.create("car"), nullptr);
    EXPECT_NE(manager.create("car"), nullptr);
    EXPECT_NE(manager.create("car"), nullptr);

    EXPECT_EQ(manager.get_size("car"), 4);
    EXPECT_EQ(manager.get_max_size("car"), 4);

    manager.destroy(1);
    manager.destroy(3);

    EXPECT_EQ(manager.get_size("car"), 2);
    EXPECT_EQ(manager.get_max_size("car"), 4);

    EXPECT_NE(manager.create("car"), nullptr);
    EXPECT_NE(manager.create("car"), nullptr);

    EXPECT_EQ(manager.get_size("car"), 4);
    EXPECT_EQ(manager.get_max_size("car"), 4);

    EXPECT_EQ(manager.at("car", 0)->get_id(), 4);
    EXPECT_EQ(manager.at("car", 1)->get_id(), 2);
    EXPECT_EQ(manager.at("car", 2)->get_id(), 5);
    EXPECT_EQ(manager.at("car", 3)->get_id(), 6);

    EXPECT_EQ(manager.at("car", 0)->get_name(), "Car");
    EXPECT_EQ(manager.at("car", 1)->get_name(), "Car");
    EXPECT_EQ(manager.at("car", 2)->get_name(), "Car");
    EXPECT_EQ(manager.at("car", 3)->get_name(), "Car");
}

TEST(manager, created_objects_has_correct_ids)
{
    auto manager = Gecko::Manager<Object, std::string, std::uint16_t>();
    auto car_factory = std::bind(Car::create, std::placeholders::_1);
    auto tank_factory = std::bind(Tank::create, std::placeholders::_1);

    EXPECT_EQ(manager.register_type<Car>("car", car_factory), true);
    EXPECT_EQ(manager.register_type<Tank>("tank", tank_factory), true);

    EXPECT_EQ(manager.allocate("car", 6), true);
    EXPECT_EQ(manager.allocate("tank", 2), true);

    EXPECT_EQ(manager.get_size("car"), 0);
    EXPECT_EQ(manager.get_size("tank"), 0);

    EXPECT_EQ(manager.get_max_size("car"), 6);
    EXPECT_EQ(manager.get_max_size("tank"), 2);

    EXPECT_NE(manager.create("car"), nullptr);
    EXPECT_NE(manager.create("car"), nullptr);

    EXPECT_NE(manager.create("tank"), nullptr);
    EXPECT_NE(manager.create("tank"), nullptr);

    EXPECT_NE(manager.create("car"), nullptr);
    EXPECT_NE(manager.create("car"), nullptr);

    EXPECT_EQ(manager.get_size("car"), 4);
    EXPECT_EQ(manager.get_size("tank"), 2);

    EXPECT_EQ(manager.get_max_size("car"), 6);
    EXPECT_EQ(manager.get_max_size("tank"), 2);

    EXPECT_EQ(manager.at("car", 0)->get_id(), 1);
    EXPECT_EQ(manager.at("car", 1)->get_id(), 2);
    EXPECT_EQ(manager.at("car", 2)->get_id(), 5);
    EXPECT_EQ(manager.at("car", 3)->get_id(), 6);

    EXPECT_EQ(manager.at("car", 0)->get_name(), "Car");
    EXPECT_EQ(manager.at("car", 1)->get_name(), "Car");
    EXPECT_EQ(manager.at("car", 2)->get_name(), "Car");
    EXPECT_EQ(manager.at("car", 3)->get_name(), "Car");

    EXPECT_EQ(manager.at("tank", 0)->get_id(), 3);
    EXPECT_EQ(manager.at("tank", 1)->get_id(), 4);

    EXPECT_EQ(manager.at("tank", 0)->get_name(), "Tank");
    EXPECT_EQ(manager.at("tank", 1)->get_name(), "Tank");
}

TEST(manager, created_and_destroyed_objects_has_correct_id)
{
    auto manager = Gecko::Manager<Object, std::string, std::uint16_t>();
    auto car_factory = std::bind(Car::create, std::placeholders::_1);
    auto tank_factory = std::bind(Tank::create, std::placeholders::_1);

    EXPECT_EQ(manager.register_type<Car>("car", car_factory), true);
    EXPECT_EQ(manager.register_type<Tank>("tank", tank_factory), true);

    EXPECT_EQ(manager.allocate("car", 8), true);
    EXPECT_EQ(manager.allocate("tank", 4), true);

    EXPECT_NE(manager.create("car"), nullptr);
    EXPECT_NE(manager.create("car"), nullptr);
    EXPECT_NE(manager.create("car"), nullptr);
    EXPECT_NE(manager.create("car"), nullptr);

    manager.destroy(1);
    manager.destroy(3);

    EXPECT_NE(manager.create("tank"), nullptr);
    EXPECT_NE(manager.create("tank"), nullptr);

    EXPECT_EQ(manager.get(2)->get_id(), 2);
    EXPECT_EQ(manager.get(4)->get_id(), 4);
    EXPECT_EQ(manager.get(5)->get_id(), 5);
    EXPECT_EQ(manager.get(6)->get_id(), 6);

    EXPECT_EQ(manager.get(2)->get_name(), "Car");
    EXPECT_EQ(manager.get(4)->get_name(), "Car");

    EXPECT_EQ(manager.get(5)->get_name(), "Tank");
    EXPECT_EQ(manager.get(6)->get_name(), "Tank");
}

TEST(manager, all_created_objects_can_be_destroyed_at_once)
{
    auto manager = Gecko::Manager<Object, std::string, std::uint16_t>();
    auto car_factory = std::bind(Car::create, std::placeholders::_1);
    auto tank_factory = std::bind(Tank::create, std::placeholders::_1);

    EXPECT_EQ(manager.register_type<Car>("car", car_factory), true);
    EXPECT_EQ(manager.register_type<Tank>("tank", tank_factory), true);

    EXPECT_EQ(manager.allocate("car", 4), true);
    EXPECT_EQ(manager.allocate("tank", 2), true);

    EXPECT_NE(manager.create("car"), nullptr);
    EXPECT_NE(manager.create("car"), nullptr);
    EXPECT_NE(manager.create("car"), nullptr);
    EXPECT_NE(manager.create("car"), nullptr);

    EXPECT_NE(manager.create("tank"), nullptr);
    EXPECT_NE(manager.create("tank"), nullptr);

    EXPECT_EQ(manager.size(), 6);

    manager.destroy_all();

    EXPECT_EQ(manager.size(), 0);
}
*/
