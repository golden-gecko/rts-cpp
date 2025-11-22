namespace Gecko
{
    class Configuration;
    class Game;
    class ComponentManager;
    class ConfigurationManager;
    class MapManager;
    class ObjectManager;
    class OrderManager;
    class PlayerManager;
    class SkillManager;
    class Statistics;
    class UI;
};

struct init_game_config
{
    bool component_manager = true;
    bool configuration_manager = true;
    bool map_manager = true;
    bool object_manager = true;
    bool order_manager = true;
    bool player_manager = true;
    bool skill_manager = true;
    bool statistics = true;
    bool ui = true;
};

struct init_game
{
    std::shared_ptr<Gecko::ComponentManager> component_manager;
    std::shared_ptr<Gecko::ConfigurationManager> configuration_manager;
    std::shared_ptr<Gecko::Game> game;
    std::shared_ptr<Gecko::MapManager> map_manager;
    std::shared_ptr<Gecko::ObjectManager> object_manager;
    std::shared_ptr<Gecko::OrderManager> order_manager;
    std::shared_ptr<Gecko::PlayerManager> player_manager;
    std::shared_ptr<Gecko::SkillManager> skill_manager;
    std::shared_ptr<Gecko::Statistics> statistics;
    std::shared_ptr<Gecko::UI> ui;

    init_game(init_game_config config);

    ~init_game();
};
