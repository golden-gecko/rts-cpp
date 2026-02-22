#include "Gecko/UI/Widgets/OrderQueue.hpp"

#include "Gecko/Containers/Orders.hpp"
#include "Gecko/Containers/Selected.hpp"
#include "Gecko/Games/Game.hpp"
#include "Gecko/Managers/ObjectManager.hpp"
#include "Gecko/Managers/OrderManager.hpp"
#include "Gecko/Objects/Object.hpp"
#include "Gecko/Orders/Order.hpp"
#include "Gecko/Players/Player.hpp"
#include "Gecko/UI/UI.hpp"

namespace Gecko
{
    void OrderQueueWidget::ProcessEvent(Rml::Event& event)
    {
    }

    void OrderQueueWidget::init_data_bindigs(Rml::Context* context)
    {
        if (Rml::DataModelConstructor constructor = context->CreateDataModel("order_queue"))
        {
            if (Rml::StructHandle handle = constructor.RegisterStruct<Order>())
            {
                handle.RegisterMember("id", &Order::id);
                handle.RegisterMember("name", &Order::name);
                handle.RegisterMember("target", &Order::target);

                constructor.RegisterArray<Rml::Vector<Order>>();
                constructor.Bind("orders", &m_orders);
                constructor.BindEventCallback("cancel", &OrderQueueWidget::cancel, this);

                m_model = constructor.GetModelHandle();
            }
        }
    }

    void OrderQueueWidget::init_events(Rml::ElementDocument* document)
    {
    }

    void OrderQueueWidget::deinit_events(Rml::ElementDocument* document)
    {
    }

    void OrderQueueWidget::cancel(Rml::DataModelHandle model, Rml::Event& ev, const Rml::VariantList& arguments)
    {
        int order_id = (arguments.size() == 1 ? arguments[0].Get<int>() : 0);

        if (PlayerPtr player = Game::getSingleton().get_active_player())
        {
            if (ObjectPtr object = ObjectManager::getSingleton().get(player->get_first_selected()))
            {
                object->get_orders()->remove(Id(order_id));
            }
        }
    }

    void OrderQueueWidget::update()
    {
        m_orders.clear();

        if (PlayerPtr player = Game::getSingleton().get_active_player())
        {
            if (ObjectPtr object = ObjectManager::getSingleton().get(player->get_first_selected()))
            {
                for (const Id& order_id : object->get_orders()->get_queue())
                {
                    if (OrderPtr order = OrderManager::getSingleton().get(order_id))
                    {
                        m_orders.push_back({ order_id.get(), order_type::to_string(order->get_type()), "N/A" });
                    }
                }
            }
        }

        m_model.DirtyVariable("orders");
    }
}
