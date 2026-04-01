#include <crtdbg.h>

#include <Gecko/Configuration.hpp>
#include <Gecko/Log.hpp>
#include <Gecko/Managers/Manager.hpp>
#include <Gecko/Utils/Time.hpp>

/*
#include "AStar.h"
#include "PathFinder.h"
*/

namespace DevAllocator
{
    class Object
    {
    public:
        Object()
        {
            // L_TRACE << "Object::Object()";
        }

        Object(const Object& other)
        {
            // L_TRACE << "Object::Object(const Object& other)";
        }

        virtual ~Object()
        {
            // L_TRACE << "Object::~Object()";
        }

        virtual void update(float time)
        {
            // L_TRACE << "Object::update()";
        }

        virtual std::string get_name() const = 0;
    };

    class Car : public Object
    {
    public:
        static Car* create(Car* memory)
        {
            return new (memory) Car();
        }

    public:
        Car()
        {
            // L_TRACE << "Car::Car()";

            data.resize(10, 8);
        }

        Car(const Car& other) :
            Object(other)
        {
            // L_TRACE << "Car::Car(const Car& other)";

            data = other.data;
        }

        ~Car() override
        {
            // L_TRACE << "Car::~Car()";
        }

        void update(float time) override
        {
            // L_TRACE << "Car::update()";
        }

        std::string get_name() const override
        {
            return "Car";
        }

    private:
        std::vector<std::uint8_t> data;
    };

    class Tank : public Object
    {
    public:
        static Tank* create(Tank* memory)
        {
            return new (memory) Tank();
        }

    public:
        Tank()
        {
            // L_TRACE << "Tank::Tank()";

            data.resize(10, 8);
        }

        Tank(const Tank& other) :
            Object(other)
        {
            // L_TRACE << "Tank::Tank(const Tank& other)";

            data = other.data;
        }

        ~Tank() override
        {
            // L_TRACE << "Tank::~Tank()";
        }

        void update(float time) override
        {
            // L_TRACE << "Tank::update()";
        }

        std::string get_name() const override
        {
            return "Tank";
        }

    private:
        std::vector<std::uint8_t> data;
    };

    void speed_allocator_with_traits(std::size_t size)
    {
        L_TIME("speed_allocator_with_traits");

        using TraitsCar = std::allocator_traits<std::allocator<Car>>;
        using TraitsTank = std::allocator_traits<std::allocator<Tank>>;

        std::allocator<Car> car_allocator;
        std::allocator<Tank> tank_allocator;

        auto car_memory = TraitsCar::allocate(car_allocator, size);
        auto tank_memory = TraitsTank::allocate(tank_allocator, size);

        for (std::size_t i = 0; i < size; i++)
        {
            TraitsCar::construct(car_allocator, car_memory + i);
            TraitsTank::construct(tank_allocator, tank_memory + i);
        }

        for (std::size_t i = 0; i < size; i++)
        {
            TraitsCar::destroy(car_allocator, car_memory + i);
            TraitsTank::destroy(tank_allocator, tank_memory + i);
        }

        TraitsCar::deallocate(car_allocator, car_memory, size);
        TraitsTank::deallocate(tank_allocator, tank_memory, size);
    }

    void speed_allocator_with_construct_at(std::size_t size)
    {
        L_TIME("speed_allocator_with_construct_at");

        std::allocator<Car> car_allocator;
        std::allocator<Tank> tank_allocator;

        auto car_memory = car_allocator.allocate(size);
        auto tank_memory = tank_allocator.allocate(size);

        for (std::size_t i = 0; i < size; i++)
        {
            std::construct_at(car_memory + i);
            std::construct_at(tank_memory + i);
        }

        for (std::size_t i = 0; i < size; i++)
        {
            std::destroy_at(car_memory + i);
            std::destroy_at(tank_memory + i);
        }

        car_allocator.deallocate(car_memory, size);
        tank_allocator.deallocate(tank_memory, size);
    }

    void speed_allocator_with_new(std::size_t size)
    {
        L_TIME("speed_allocator_with_new");

        std::allocator<Car> car_allocator;
        std::allocator<Tank> tank_allocator;

        auto car_memory = car_allocator.allocate(size);
        auto tank_memory = tank_allocator.allocate(size);

        for (std::size_t i = 0; i < size; i++)
        {
            new (car_memory + i) Car();
            new (tank_memory + i) Tank();
        }

        for (std::size_t i = 0; i < size; i++)
        {
            (car_memory + i)->~Car();
            (tank_memory + i)->~Tank();
        }

        car_allocator.deallocate(car_memory, size);
        tank_allocator.deallocate(tank_memory, size);
    }

    void speed_allocator_with_copy_constructor(std::size_t size)
    {
        L_TIME("speed_allocator_with_copy_constructor");

        std::allocator<Car> car_allocator;
        std::allocator<Tank> tank_allocator;

        auto car_memory = car_allocator.allocate(size);
        auto tank_memory = tank_allocator.allocate(size);

        new (car_memory) Car();
        new (tank_memory) Tank();

        for (std::size_t i = 1; i < size; i++)
        {
            new (car_memory + i) Car(*car_memory);
            new (tank_memory + i) Tank(*tank_memory);
        }

        for (std::size_t i = 0; i < size; i++)
        {
            (car_memory + i)->~Car();
            (tank_memory + i)->~Tank();
        }

        car_allocator.deallocate(car_memory, size);
        tank_allocator.deallocate(tank_memory, size);
    }

    void speed_allocator_with_custom_methods(std::size_t size)
    {
        L_TIME("speed_allocator_with_custom_methods");

        std::allocator<Car> car_allocator;
        std::allocator<Tank> tank_allocator;

        auto car_memory = car_allocator.allocate(size);
        auto tank_memory = tank_allocator.allocate(size);

        for (std::size_t i = 0; i < size; i++)
        {
            Car::create(car_memory + i);
            Tank::create(tank_memory + i);
        }

        for (std::size_t i = 0; i < size; i++)
        {
            (car_memory + i)->~Car();
            (tank_memory + i)->~Tank();
        }

        car_allocator.deallocate(car_memory, size);
        tank_allocator.deallocate(tank_memory, size);
    }

    void speed_vector(std::size_t size)
    {
        L_TIME("speed_vector");

        std::vector<Car> cars(size, Car());
        std::vector<Tank> tanks(size, Tank());
    }

    void run()
    {
        std::size_t size = 1000000;

        speed_allocator_with_traits(size);
        speed_allocator_with_construct_at(size);
        speed_allocator_with_new(size);
        speed_allocator_with_copy_constructor(size);
        speed_allocator_with_custom_methods(size);
        speed_vector(size);
    }
}

namespace DevManager
{
    class Vehicle
    {
    public:
        void init()
        {
        }

        void deinit()
        {
        }

        Vehicle(const std::shared_ptr<Gecko::Configuration>& configuration)
        {
            m_wheels = configuration->get_int("wheels");
        }

        Vehicle(const Vehicle& other) :
            m_wheels(other.m_wheels)
        {
        }

        virtual ~Vehicle()
        {
        }

        std::uint16_t get_id() const
        {
            return m_id;
        }

        std::uint16_t get_wheels() const
        {
            return m_wheels;
        }

        void set_id(std::uint16_t id)
        {
            m_id = id;
        }

    private:
        std::uint16_t m_id = 0;
        std::uint16_t m_wheels = 0;
    };

    class Bus : public Vehicle
    {
    public:
        static Bus* create(Bus* memory, const std::shared_ptr<Gecko::Configuration>& configuration)
        {
            return new (memory) Bus(configuration);
        }

        Bus(const std::shared_ptr<Gecko::Configuration>& configuration) :
            Vehicle(configuration)
        {
        }

        Bus(const Bus& other) :
            Vehicle(other)
        {
        }

        virtual ~Bus()
        {
        }
    };

    class Truck : public Vehicle
    {
    public:
        static Truck* create(Truck* memory, const std::shared_ptr<Gecko::Configuration>& configuration)
        {
            return new (memory) Truck(configuration);
        }

        Truck(const std::shared_ptr<Gecko::Configuration>& configuration) :
            Vehicle(configuration)
        {
        }

        Truck(const Truck& other) :
            Vehicle(other)
        {
        }

        virtual ~Truck()
        {
        }
    };

    void run()
    {
        /*
        auto l1 = [&]()
        {
            return Bus::create(configuration);
        };

        auto l2 = [&](auto memory)
        {
            return Bus::create(memory, configuration);
        };

        auto v1 = static_cast<Bus*(*)(std::shared_ptr<Gecko::Configuration>)>(&Bus::create);
        auto v2 = static_cast<Bus*(*)(Bus* memory, std::shared_ptr<Gecko::Configuration>)>(&Bus::create);

        typedef std::function<Bus*(Bus*)> Factory;

        Factory v3 = std::bind(v1, configuration);
        Factory v4 = std::bind(v2, std::placeholders::_1, configuration);

        std::map<std::string, Factory> factories;

        factories.emplace("bus", v3);
        factories.emplace("truck", v4);

        auto v1 = static_cast<void(*)(std::shared_ptr < Gecko::Configuration>)>(&create);
        auto v2 = static_cast<void(*)(Bus*, std::shared_ptr<Gecko::Configuration>)>(&create);

        auto f1 = std::bind(&Bus::create, configuration);
        auto f2 = std::bind(&Bus::create, std::placeholders::_1, configuration);

        auto o1 = create<Bus>(configuration);
        auto o2 = create<Bus>(memory, configuration);

        auto o3 = v1(configuration);
        auto o4 = v2(memory, configuration);

        auto o5 = factories["bus"](memory);
        auto o6 = factories["truck"](memory);

        std::free(memory);
        */

        auto bus_configuration = std::make_shared<Gecko::Configuration>();
        auto city_bus_configuration = std::make_shared<Gecko::Configuration>();
        auto truck_configuration = std::make_shared<Gecko::Configuration>();
        auto big_truck_configuration = std::make_shared<Gecko::Configuration>();

        bus_configuration->set("wheels", 4);
        city_bus_configuration->set("wheels", 6);
        truck_configuration->set("wheels", 8);
        big_truck_configuration->set("wheels", 12);

        auto bus_factory = std::bind(Bus::create, std::placeholders::_1, city_bus_configuration);
        auto city_bus_factory = std::bind(Bus::create, std::placeholders::_1, city_bus_configuration);
        auto truck_factory = std::bind(Truck::create, std::placeholders::_1, truck_configuration);
        auto big_truck_factory = std::bind(Truck::create, std::placeholders::_1, big_truck_configuration);

        auto manager = std::make_shared<Gecko::Manager<Vehicle, std::string, std::uint16_t>>();

        manager->register_type<Bus>("bus", bus_factory);
        manager->register_type<Bus>("city_bus", city_bus_factory);
        manager->register_type<Truck>("truck", truck_factory);
        manager->register_type<Truck>("big_truck", big_truck_factory);

        manager->allocate("bus", 10);
        manager->allocate("city_bus", 10);
        manager->allocate("truck", 10);
        manager->allocate("big_truck", 10);

        auto v0 = manager->create("bus");
        auto v1 = manager->create("city_bus");
        auto v2 = manager->create("truck");
        auto v3 = manager->create("big_truck");

        std::cout << v0->get_id() << " " << v0->get_wheels() << std::endl;
        std::cout << v1->get_id() << " " << v1->get_wheels() << std::endl;
        std::cout << v2->get_id() << " " << v2->get_wheels() << std::endl;
        std::cout << v3->get_id() << " " << v3->get_wheels() << std::endl;

        manager->destroy(v0->get_id());
        manager->destroy(v1->get_id());
        manager->destroy(v2->get_id());
        manager->destroy(v3->get_id());

        manager->deallocate_all();
    }
}

namespace DevNavigation
{
    class MyNode :
        public AStarNode
    {
    // From AStarNode.
    public:
        float distanceTo(AStarNode* node) const override;
    };

    float MyNode::distanceTo(AStarNode* node) const
    {
        return 1.0f;
    }

    void run()
    {
        PathFinder<MyNode> myFinder; // Used to find a path in a MyNode graph

        std::vector<MyNode*> path;
        MyNode nodes[100];

        for (int i = 0; i < 100; ++i)
        {
            nodes[i].setPosition(i, i);

            if (i + 1 < 100)
            {
                nodes[i].addChild(&nodes[i + 1], 1.0f);
            }

            // Do your stuff here to link nodes between them as needed
            // by using Node::addChild().
            // This is the "setup" of your graph.
        }

        // Let's say we want the path from the first node to the last one ...
        myFinder.setStart(nodes[0]);
        myFinder.setGoal(nodes[99]);

        // ... and that's it !
        bool result = myFinder.findPath<AStar>(path); // Just specify the algorithm you want to use

        if (result)
        {
            std::cout << "Success ! A path has been found." << std::endl;

            for (const auto& node : path)
            {
                std::cout << node->getX() << ":" << node->getY() << std::endl;
            }
        }
        else
        {
            std::cout << "Erk, it seems there is no way through." << std::endl;
        }
    }
}

int main()
{
    // DevAllocator::run();
    DevManager::run();
    // DevNavigation::run();

    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    std::cin.ignore();

    return 0;
}
