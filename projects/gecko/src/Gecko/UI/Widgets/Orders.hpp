#pragma once

#include "Gecko/UI/Widgets/Widget.hpp"

namespace Gecko
{
    class OrdersWidget :
        public Widget
    {
	// From Widget.
    public:
        void init_data_bindigs(Rml::Context* context) override;

    // OrdersWidget.
    public:
        void select(Rml::DataModelHandle model, Rml::Event& ev, const Rml::VariantList& arguments);
        void select(const std::string configuration);
        void update(const std::set<std::string>& orders);

    public:
        const std::string& get_selected() const
        {
            return m_selected_order;
        }

    private:
        struct Order
        {
            std::string name;
        };

        Rml::Vector<Order> m_orders;
        std::string m_selected_order;
    };
}
