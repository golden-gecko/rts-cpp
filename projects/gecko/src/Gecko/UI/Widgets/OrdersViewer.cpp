#include "Gecko/UI/Widgets/OrdersViewer.hpp"

#include "Gecko/Managers/ObjectManager.hpp"
#include "Gecko/Managers/OrderManager.hpp"
#include "Gecko/Objects/Object.hpp"
#include "Gecko/Orders/Order.hpp"

namespace Gecko
{
    void OrdersViewerWidget::init_data_bindigs(Rml::Context* context)
    {
        if (Rml::DataModelConstructor constructor = context->CreateDataModel("orders_viewer"))
        {
            if (Rml::StructHandle handle = constructor.RegisterStruct<Order>())
            {
                handle.RegisterMember("id", &Order::id);
                handle.RegisterMember("name", &Order::name);
                handle.RegisterMember("sender_name", &Order::sender_name);
                handle.RegisterMember("receiver_name", &Order::receiver_name);
                handle.RegisterMember("attempts", &Order::attempts);
                handle.RegisterMember("max_attempts", &Order::max_attempts);

                constructor.RegisterArray<Rml::Vector<Order>>();
                constructor.Bind("orders", &m_orders);
                constructor.BindEventCallback("close", &OrdersViewerWidget::close, this);

                m_model = constructor.GetModelHandle();
            }
        }
    }

    void OrdersViewerWidget::close(Rml::DataModelHandle model, Rml::Event& ev, const Rml::VariantList& arguments)
    {
        if (m_document)
        {
            std::string id = (arguments.size() == 1 ? arguments[0].Get<std::string>() : "");

            if (Rml::Element* element = m_document->GetElementById(id))
            {
                element->SetClass("hidden", true);
            }
        }
    }

    void OrdersViewerWidget::update()
    {
        m_orders.clear();

        for (const auto& [id, order] : OrderManager::getSingleton())
        {
            std::string sender_name;
            std::string receiver_name;

            if (ObjectPtr sender = ObjectManager::getSingleton().get(order->get_sender_id()))
            {
                sender_name = sender->get_name();
            }

            if (ObjectPtr receiver = ObjectManager::getSingleton().get(order->get_receiver_id()))
            {
                receiver_name = receiver->get_name();
            }

            m_orders.push_back({ id.get(), order_type::to_string(order->get_type()), sender_name, receiver_name, order->get_attempts_to_complete(), order->get_max_attempts_to_complete() });
        }

        m_model.DirtyVariable("orders");
    }
}
