#include "Gecko/Orders/OrderMove.hpp"

#include "Gecko/Configuration.hpp"
#include "Gecko/Games/Game.hpp"
#include "Gecko/Layers/Layer.hpp"
#include "Gecko/Managers/ObjectManager.hpp"
#include "Gecko/Maps/Map.hpp"
#include "Gecko/UI/Indicators/Path.hpp"

namespace Gecko
{
    OrderMove* OrderMove::create()
    {
        return new OrderMove();
    }

    OrderMove* OrderMove::create(OrderMove* memory)
    {
        return new (memory) OrderMove();
    }

    OrderMove* OrderMove::create(OrderMove* memory, const ConfigurationPtr& configuration)
    {
        auto order = new (memory) OrderMove();

        order->deserialize(configuration);

        return order;
    }

    OrderMove::OrderMove() :
        base_type(order_type::Value::Move)
    {
    }

    void OrderMove::init()
    {
        base_type::init();

        set_target_position(Ogre::Vector3::ZERO);
        set_path(Navigation::Path());
    }

    ConfigurationPtr OrderMove::serialize() const
    {
        auto configuration = base_type::serialize();

        configuration->set("target_position", target_position);
        configuration->set("path", path);

        return configuration;
    }

    void OrderMove::deserialize(const ConfigurationPtr& configuration)
    {
        base_type::deserialize(configuration);

        target_position = configuration->get_vector3("target_position", Ogre::Vector3::ZERO);
        path = configuration->get_path("path", Navigation::Path());
    }

    std::vector<std::shared_ptr<Indicator>> OrderMove::generate_indicators(int order_number) const
    {
        if (MapPtr map = Game::getSingleton().get_active_map())
        {
            if (LayerPtr layer = map->get_layer(Settings::Layer::Terrain))
            {
                std::shared_ptr<Path> indicator = std::make_shared<Path>();

                indicator->set_material_name(std::format("path_{}", order_number));
                indicator->set_points(std::format("path_{}", order_number), get_path().get_points());

                return { indicator };
            }
        }

        return {};
    }
}
