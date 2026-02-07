#include "Gecko/UI/Widgets/Orders.hpp"

namespace Gecko
{
    void OrdersWidget::ProcessEvent(Rml::Event& event)
    {
    }

    void OrdersWidget::init_data_bindigs(Rml::Context* context)
    {
        if (Rml::DataModelConstructor constructor = context->CreateDataModel("orders"))
        {
            if (Rml::StructHandle handle = constructor.RegisterStruct<Order>())
            {
                handle.RegisterMember("name", &Order::name);

                constructor.RegisterArray<Rml::Vector<Order>>();
                constructor.Bind("orders", &m_orders);
                constructor.Bind("selected_order", &m_selected_order);

                m_model = constructor.GetModelHandle();
            }
        }
    }

    void OrdersWidget::init_events(Rml::ElementDocument* document)
    {
    }

    void OrdersWidget::deinit_events(Rml::ElementDocument* document)
    {
    }

    void OrdersWidget::update(const std::set<std::string>& orders)
    {
        m_orders.clear();

        for (const auto& i : orders)
        {
            m_orders.push_back({ i });
        }

        m_model.DirtyVariable("orders");
    }
}
