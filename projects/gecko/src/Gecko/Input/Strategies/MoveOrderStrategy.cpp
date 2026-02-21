#include "Gecko/Input/Strategies/MoveOrderStrategy.hpp"

#include "Gecko/Containers/Orders.hpp"
#include "Gecko/Containers/Selected.hpp"
#include "Gecko/Games/Game.hpp"
#include "Gecko/Input/Input.hpp"
#include "Gecko/Log.hpp"
#include "Gecko/Managers/ObjectManager.hpp"
#include "Gecko/Managers/OrderManager.hpp"
#include "Gecko/Objects/Object.hpp"
#include "Gecko/Players/Player.hpp"
#include "Gecko/Utils/Raycast.hpp"

namespace Gecko
{
    void MoveOrderStrategy::execute(const OIS::MouseEvent& arg)
    {
        PlayerPtr player = Game::getSingleton().get_active_player();

        if (player == nullptr || player->get_selected()->size() <= 0)
        {
            return;
        }

        LayerRaycastResults layer_result = Utils::Raycast::to_layer(Game::getSingleton().get_map_scene(), arg);

        if (layer_result.has_value() == false)
        {
            return;
        }

        for (const Id& object_id : *(player->get_selected()))
        {
            ObjectPtr object = ObjectManager::getSingleton().get(object_id);

            if (object == nullptr)
            {
                continue;
            }

            if (Input::getSingleton().is_key_pressed(Command::Value::Multiple_Order) == false)
            {
                object->get_orders()->remove_all_orders();
            }

            if (OrderPtr order = OrderManager::getSingleton().order_move(Id::Empty, object_id, layer_result->second))
            {
                object->get_orders()->add_last(order->get_id());
            }
            else
            {
                L_WARNING << "Could not create order.";
            }
        }
    }
}
