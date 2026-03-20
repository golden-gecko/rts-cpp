#include "Gecko/UI/Widgets/Orders.hpp"

namespace Gecko
{
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
                constructor.BindEventCallback("select", &OrdersWidget::select, this);

                m_model = constructor.GetModelHandle();
            }
        }
    }

    void OrdersWidget::select(Rml::DataModelHandle model, Rml::Event& ev, const Rml::VariantList& arguments)
    {
        select(arguments.size() == 1 ? arguments[0].Get<std::string>() : "");
    }

    void OrdersWidget::select(const std::string order)
    {
        m_selected_order = order;

        m_model.DirtyVariable("selected_order");
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
