/*
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/labeled_graph.hpp>
#include <boost/graph/topological_sort.hpp>

namespace
{
    class Object
    {
    public:
        Object()
        {
            LOG("Object::Object()");
        }

        Object(const Object& other)
        {
            LOG("Object::Object(const Object& other)");
        }

        virtual ~Object()
        {
            LOG("Object::~Object()");
        }

        virtual void update(float time)
        {
            LOG("Object::update()");
        }

        virtual std::string get_name() const = 0;

        std::uint16_t get_id() const
        {
            return id;
        }

        void set_id(const std::uint16_t& _id)
        {
            id = _id;
        }

    private:
        std::uint16_t id = 0;
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
            LOG("Car::Car()");

            data.resize(10, 8);
        }

        Car(const Car& other) :
            Object(other)
        {
            LOG("Car::Car(const Car& other)");

            data = other.data;
        }

        ~Car() override
        {
            LOG("Car::~Car()");
        }

        void update(float time) override
        {
            LOG("Car::update()");
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
            LOG("Tank::Tank()");
        }

        Tank(const Tank& other) :
            Object(other)
        {
            LOG("Tank::Tank(const Tank& other)");
        }

        ~Tank() override
        {
            LOG("Tank::~Tank()");
        }

        void update(float time) override
        {
            LOG("Tank::update()");
        }

        std::string get_name() const override
        {
            return "Tank";
        }
    };

    class Vehicle
    {
    public:
        explicit Vehicle()
        {
        }

        explicit Vehicle(std::shared_ptr<Gecko::Configuration> configuration)
        {
        }

        explicit Vehicle(const Vehicle& other)
        {
        }

        virtual ~Vehicle()
        {
        }

        void init()
        {
        }

        void deinit()
        {
        }

    public:
        auto get_id() const
        {
            return id;
        }

    public:
        void set_id(std::uint8_t _id)
        {
            id = _id;
        }

    private:
        std::uint8_t id = 0;
    };

    class Bus : public Vehicle
    {
    public:
        static Bus* create()
        {
            return new Bus();
        }

        static Bus* create(std::shared_ptr<Gecko::Configuration> configuration)
        {
            return new Bus(configuration);
        }

        explicit Bus()
        {
        }

        explicit Bus(std::shared_ptr<Gecko::Configuration> configuration) :
            Vehicle(configuration)
        {
        }

        explicit Bus(const Bus& other) :
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
        static Truck* create()
        {
            return new Truck();
        }

        static Truck* create(std::shared_ptr<Gecko::Configuration> configuration)
        {
            return new Truck(configuration);
        }

        static Truck* create(std::shared_ptr<Gecko::Configuration> configuration, std::uint8_t default_wheels)
        {
            return new Truck(configuration, default_wheels);
        }

        explicit Truck()
        {
        }

        explicit Truck(std::shared_ptr<Gecko::Configuration> configuration) :
            Vehicle(configuration)
        {
            wheels = configuration->get_int("wheels", 0);
        }

        explicit Truck(std::shared_ptr<Gecko::Configuration> configuration, std::uint8_t default_wheels) :
            Vehicle(configuration)
        {
            wheels = configuration->get_int("wheels", default_wheels);
        }

        explicit Truck(const Truck& other) :
            Vehicle(other)
        {
            wheels = other.wheels;
        }

        virtual ~Truck()
        {
        }

    private:
        std::uint8_t wheels = 0;
    };

    void speed_allocator(std::size_t size)
    {
        Gecko::Time::Scope time("allocator");

        std::allocator<Car> car_allocator;
        std::allocator<Tank> tank_allocator;

        auto car_memory = car_allocator.allocate(size);
        auto tank_memory = tank_allocator.allocate(size);

        for (std::size_t i = 0; i < size; ++i)
        {
            car_allocator.construct(car_memory + i);
            tank_allocator.construct(tank_memory + i);
        }

        for (std::size_t i = 0; i < size; ++i)
        {
            car_allocator.destroy(car_memory + i);
            tank_allocator.destroy(tank_memory + i);
        }

        car_allocator.deallocate(car_memory, size);
        tank_allocator.deallocate(tank_memory, size);
    }

    void speed_allocator_with_construct_at(std::size_t size)
    {
        Gecko::Time::Scope time("allocator with construct at");

        std::allocator<Car> car_allocator;
        std::allocator<Tank> tank_allocator;

        auto car_memory = car_allocator.allocate(size);
        auto tank_memory = tank_allocator.allocate(size);

        for (std::size_t i = 0; i < size; ++i)
        {
            std::construct_at(car_memory + i);
            std::construct_at(tank_memory + i);
        }

        for (std::size_t i = 0; i < size; ++i)
        {
            std::destroy_at(car_memory + i);
            std::destroy_at(tank_memory + i);
        }

        car_allocator.deallocate(car_memory, size);
        tank_allocator.deallocate(tank_memory, size);
    }

    void speed_allocator_with_new(std::size_t size)
    {
        Gecko::Time::Scope time("allocator with new");

        std::allocator<Car> car_allocator;
        std::allocator<Tank> tank_allocator;

        auto car_memory = car_allocator.allocate(size);
        auto tank_memory = tank_allocator.allocate(size);

        for (std::size_t i = 0; i < size; ++i)
        {
            new (car_memory + i) Car();
            new (tank_memory + i) Tank();
        }

        for (std::size_t i = 0; i < size; ++i)
        {
            (car_memory + i)->update(0.0f);
            (tank_memory + i)->update(0.0f);
        }

        for (std::size_t i = 0; i < size; ++i)
        {
            (car_memory + i)->~Car();
            (tank_memory + i)->~Tank();
        }

        car_allocator.deallocate(car_memory, size);
        tank_allocator.deallocate(tank_memory, size);
    }

    void speed_allocator_with_copy_constructor(std::size_t size)
    {
        Gecko::Time::Scope time("allocator with copy constructor");

        std::allocator<Car> car_allocator;
        std::allocator<Tank> tank_allocator;

        auto car_memory = car_allocator.allocate(size);
        auto tank_memory = tank_allocator.allocate(size);

        new (car_memory) Car();
        new (tank_memory) Tank();

        for (std::size_t i = 1; i < size; ++i)
        {
            new (car_memory + i) Car(*car_memory);
            new (tank_memory + i) Tank(*tank_memory);
        }

        for (std::size_t i = 0; i < size; ++i)
        {
            (car_memory + i)->update(0.0f);
            (tank_memory + i)->update(0.0f);
        }

        for (std::size_t i = 0; i < size; ++i)
        {
            (car_memory + i)->~Car();
            (tank_memory + i)->~Tank();
        }

        car_allocator.deallocate(car_memory, size);
        tank_allocator.deallocate(tank_memory, size);
    }

    void speed_allocator_with_custom_methods(std::size_t size)
    {
        Gecko::Time::Scope time("allocator with methods");

        std::allocator<Car> car_allocator;
        std::allocator<Tank> tank_allocator;

        auto car_memory = car_allocator.allocate(size);
        auto tank_memory = tank_allocator.allocate(size);

        Car::create(car_memory);
        Tank::create(tank_memory);

        for (std::size_t i = 1; i < size; ++i)
        {
            Car::create(car_memory + i);
            Tank::create(tank_memory + i);
        }

        for (std::size_t i = 0; i < size; ++i)
        {
            (car_memory + i)->update(0.0f);
            (tank_memory + i)->update(0.0f);
        }

        for (std::size_t i = 0; i < size; ++i)
        {
            (car_memory + i)->~Car();
            (tank_memory + i)->~Tank();
        }

        car_allocator.deallocate(car_memory, size);
        tank_allocator.deallocate(tank_memory, size);
    }

    void speed_vector(std::size_t size)
    {
        Gecko::Time::Scope time("vector");

        std::vector<Car> cars(size, Car());
        std::vector<Tank> tanks(size, Tank());
    }
}

template<typename BaseType>
class Factory
{
public:
    template<typename ... Arguments>
    BaseType* create(const std::string& name, Arguments&& ... arguments)
    {
        if (name == "Bus")
        {
            return Bus::create(std::forward<Arguments>(arguments)...);
        }

        if (name == "Truck")
        {
            return Truck::create(std::forward<Arguments>(arguments)...);
        }

        return nullptr;
    }
};

class Foo
{
public:
    static void print_1(int a)
    {
        std::cout << "1: " << a << std::endl;
    }

    static void print_1(int a, int b)
    {
        std::cout << "1: " << a << " 2: " << b << std::endl;
    }
};

class Bar
{
public:
    static void print_1(int a)
    {
        std::cout << "1: " << a << std::endl;
    }

    static void print_1(int a, int b)
    {
        std::cout << "1: " << a << " 2: " << b << std::endl;
    }
};

template<typename ... Arguments>
void print(Arguments&& ... arguments)
{
    Foo::print_1(std::forward<Arguments>(arguments) ...);
}

struct position_t
{
    position_t(float x, float y)
    {
    }
};

class order
{
};

class order_attack : public order
{
public:
    order_attack(int sender, int receiver, int target)
    {
        int a = 0;
    }
};

class order_move : public order
{
public:
    order_move(int sender, int receiver, float x, float y)
    {
        int a = 0;
    }
};

template <typename U>
std::true_type test(U);

std::false_type test(...);

template <typename T, typename... Ts>
std::false_type test_has_ctor(...);

template <typename T, typename... Ts>
auto test_has_ctor(T*) -> decltype(test(declval< decltype(T(declval<Ts>()...)) >()));

template <typename Type, typename ... Arguments>
std::unique_ptr<Type> create(Arguments&& ... arguments)
{
    return std::make_unique<Type>(std::forward<Arguments>(arguments) ...);
}

template <typename Type, typename ... Arguments>
std::enable_if_t<decltype(test_has_ctor<Type, Arguments ...>(nullptr))::value, std::unique_ptr<Type>>
create(Arguments&& ... arguments)
{
    return std::make_unique<Type>(std::forward<Arguments>(arguments) ...);
}

template<typename ... Arguments>
std::unique_ptr<order> create_order(const std::string& name, Arguments&& ... arguments)
{
    if (name == "attack")
    {
        return create<order_attack, Arguments ...>(std::forward<Arguments>(arguments) ...);
    }

    if (name == "move")
    {
        return create<order_move, Arguments ...>(std::forward<Arguments>(arguments) ...);
    }

    return nullptr;
}

class IRenderer
{
};

class GLRenderer : public IRenderer
{
public:
    explicit GLRenderer(int p1)
    {
        int a = 0;
    }
};

class DXRenderer : public IRenderer
{
public:
    explicit DXRenderer(int p1, int p2)
    {
        int a = 0;
    }
};

template <typename Concrete, typename... Ts>
unique_ptr<Concrete> construct(Ts&&... params)
{
    return nullptr;
}

template <typename U>
std::true_type test(U);

std::false_type test(...);

template <typename T, typename... Ts>
std::false_type test_has_ctor(...);

template <typename T, typename... Ts>
auto test_has_ctor(T*) -> decltype(test(declval<decltype(T(declval<Ts>()...))>()));

template <typename Concrete, typename... Ts>
enable_if_t<decltype(test_has_ctor<Concrete, Ts...>(nullptr))::value, unique_ptr<Concrete>> construct(Ts&&... params)
{
    return std::make_unique<Concrete>(std::forward<Ts>(params)...);
}

template <typename Concrete, typename... Ts>
enable_if_t<std::is_constructible<Concrete, Ts ...>::value, unique_ptr<Concrete>> construct(Ts&& ... params)
{
    return std::make_unique<Concrete>(std::forward<Ts>(params) ...);
}

template <typename Concrete, typename ... Ts>
unique_ptr<IRenderer> construct()
{
    return std::make_unique<Concrete>();
}

template <typename ... Ts>
unique_ptr<IRenderer> create(const string &name, Ts&& ... params)
{
    if (name == "GL")
        return construct<GLRenderer, Ts ...>(forward<Ts>(params) ...);
    else if (name == "DX")
        return construct<DXRenderer, Ts ...>(forward<Ts>(params) ...);

    return nullptr;
}

template<typename = typename std::enable_if<std::is_constructible<GLRenderer, int>::value>::type>
std::shared_ptr<GLRenderer> create(int p1)
{
    return std::make_shared<GLRenderer>(p1);
}
*/

// https://www.bfilipek.com/2016/03/nice-c-factory-implementation-2.html

/*
class vertex_t
{
public:
    std::string configuration_name;

public:
    vertex_t() = default;
    vertex_t(const vertex_t& other) = default;
    vertex_t(const std::string& configuration_name);
};

vertex_t::vertex_t(const std::string& configuration_name) :
    configuration_name(configuration_name)
{
}

inline std::ostream& operator<<(std::ostream& out, const vertex_t& value)
{
    return out << value.configuration_name;
}

typedef // boost::labeled_graph<
    boost::adjacency_list<
        boost::vecS,
        boost::vecS,
        boost::undirectedS,
        vertex_t,
        boost::property<
            boost::edge_weight_t,
            int
        >
    // >, std::string
> graph_t;

typedef boost::graph_traits<graph_t>::vertex_descriptor vertex_descriptor;
typedef std::pair<int, int> Edge;
*/

int main()
{
    /*
    for (int i = 0; i < 40; ++i)
    {
        std::cout << i << " " << ((i & (i - 1)) == 0) << std::endl;
    }

    // Graph created from the list of edges
    graph_t g; // (edge_array, edge_array + num_arcs, weights, num_nodes);

    auto v0 = boost::add_vertex("optics", vertex_t("Optics"), g);
    auto v1 = boost::add_vertex("lasers", vertex_t("Lasers"), g);
    auto v2 = boost::add_vertex("laser_gun", vertex_t("Laser Gun"), g);

    auto e0 = boost::add_edge(v0, v1, g);
    auto e1 = boost::add_edge(v1, v2, g);

    auto v3 = boost::add_vertex("laser_gun", vertex_t("Coal"), g);
    auto v4 = boost::add_vertex("laser_gun", vertex_t("Iron Ore"), g);
    auto v5 = boost::add_vertex("laser_gun", vertex_t("Iron"), g);

    auto e2 = boost::add_edge(v3, v5, g);
    auto e3 = boost::add_edge(v4, v5, g);

    boost::print_graph(g, boost::get(boost::vertex_all, g));

    std::cout << std::endl;

    // Create property_map from edges to weights
    boost::property_map<graph_t, boost::edge_weight_t>::type weightmap = get(boost::edge_weight, g);

    // Create vectors to store the predecessors (p) and the distances from the root (d)
    std::vector<vertex_descriptor> p(boost::num_vertices(g));
    std::vector<int> d(boost::num_vertices(g));

    // Create descriptor for the source node
    vertex_descriptor s = boost::vertex(v2, g);
    vertex_descriptor goal = boost::vertex(v0, g);

    // Evaluate Dijkstra on graph g with source s, predecessor_map p and distance_map d
    boost::dijkstra_shortest_paths(g, s, boost::predecessor_map(&p[0]).distance_map(&d[0]));

    //p[] is the predecessor map obtained through dijkstra
    //name[] is a vector with the names of the vertices
    //s and goal are vertex descriptors
    std::vector<boost::graph_traits<graph_t>::vertex_descriptor > path;
    boost::graph_traits<graph_t>::vertex_descriptor current = goal;

    bool found = true;

    while (current != s)
    {
        path.push_back(current);

        if (current == p[current])
        {
            found = false;

            break;
        }

        current = p[current];
    }

    // Prints the path obtained in reverse
    // std::cout << "Path from " << g["laser_gun"].configuration_name << " to " << g["optics"].configuration_name << std::endl;

    if (found)
    {
        path.push_back(s);

        for (auto it = path.crbegin(); it != path.crend(); ++it)
        {
            std::cout << g[*it].configuration_name << " -> ";
        }
    }
    else
    {
        std::cout << "not found";
    }

    std::cout << std::endl;
    */

    /*
    typedef boost::labeled_graph<boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, vertex_t>, std::string> graph_t;
    typedef boost::graph_traits<graph_t>::vertex_descriptor vertex_descriptor;

    graph_t graph;

    auto v0 = boost::add_vertex("optics", vertex_t("Optics"), graph);
    auto v1 = boost::add_vertex("lasers", vertex_t("Lasers"), graph);
    auto v2 = boost::add_vertex("laser_gun", vertex_t("Laser Gun"), graph);

    auto e1 = boost::add_edge(v0, v1, graph);
    auto e2 = boost::add_edge(v1, v2, graph);

    std::vector<vertex_descriptor> container;
    boost::topological_sort(graph.graph(), std::back_inserter(container));

    for (auto i = container.rbegin(); i != container.rend(); ++i)
    {
        std::cout << *i << " ";
    }

    std::cout << std::endl;

    std::vector<vertex_descriptor> p(boost::num_vertices(graph));
    std::vector<int> d(boost::num_vertices(graph));

    vertex_descriptor start = v0;
    vertex_descriptor goal = v2;

    boost::dijkstra_shortest_paths(graph, start, boost::predecessor_map(&p[0]).distance_map(&d[0]));

    std::vector<boost::graph_traits<graph_t>::vertex_descriptor > path;
    boost::graph_traits<graph_t>::vertex_descriptor current = goal;

    while (current != start)
    {
        path.push_back(current);
        current = p[current];
    }

    path.push_back(start);

    std::cout << std::endl;
    */

    // create(1);

    // std::cout << std::is_constructible<GLRenderer, int>::value << std::endl;
    // std::cout << std::is_constructible<GLRenderer, int, int>::value << std::endl;

    // auto pGL = create("GL", 10);
    // auto pDX = create("DX", 1024, 1024);

    // create_order("attack", 2, 4, 6);
    // create_order("move", 2, 4, 1.0f, 2.0f);

    /*
    print(666);
    print(777, 888);
    */

    /*
    auto f = std::make_shared<Factory<Vehicle>>();

    auto b1 = f->create("bus");
    auto b2 = f->create("bus", std::make_shared<Gecko::Configuration>());

    auto t1 = f->create("truck");
    auto t2 = f->create("truck", std::make_shared<Gecko::Configuration>());
    auto t3 = f->create("truck", std::make_shared<Gecko::Configuration>(), 12);
    */

    /*
    using T1 = void(*)(std::string);
    using T2 = void(*)(std::string, std::size_t);

    auto f1 = static_cast<T1>(&create);
    auto f2 = static_cast<T2>(&create);

    auto c1 = std::bind(f1, std::placeholders::_1);
    auto c2 = std::bind(f2, std::placeholders::_1, std::placeholders::_2);

    std::vector<decltype(c1)> factories;

    factories.emplace_back(c1);
    factories.emplace_back(c2);
    */

    /*
    auto file_name = "../objects/plants/tree.json";
    auto configuration = std::make_shared<Gecko::Configuration>(file_name);
    auto memory = static_cast<Bus*>(std::malloc(sizeof(Bus)));

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

    auto bus_configuration = std::make_shared<Gecko::Configuration>();
    auto truck_configuration = std::make_shared<Gecko::Configuration>();
    auto big_truck_configuration = std::make_shared<Gecko::Configuration>();

    truck_configuration->set("wheels", 4);
    big_truck_configuration->set("wheels", 8);

    auto bus_factory = std::bind(Bus::create, std::placeholders::_1, bus_configuration);
    auto truck_factory = std::bind(Truck::create, std::placeholders::_1, truck_configuration);
    auto big_truck_factory = std::bind(Truck::create, std::placeholders::_1, big_truck_configuration);

    auto manager = std::make_shared<Gecko::Manager<Vehicle, std::string, std::uint8_t>>();

    manager->register_type<Bus>("bus", bus_factory);
    manager->register_type<Truck>("truck", truck_factory);
    manager->register_type<Truck>("big_truck", big_truck_factory);

    manager->allocate("bus", 10);
    manager->allocate("truck", 10);
    manager->allocate("big_truck", 10);

    auto v0 = manager->create("bus");
    auto v1 = manager->create("truck");
    auto v2 = manager->create("big_truck");
    */

    /*
    std::size_t size = 1000000;

    speed_allocator(size);
    speed_allocator_with_construct_at(size);
    speed_allocator_with_new(size);
    speed_allocator_with_copy_constructor(size);
    speed_allocator_with_custom_methods(size);
    speed_vector(size);
    */

    // _CrtDumpMemoryLeaks();

    return 0;
}
