#pragma once

#include "Gecko/UI/Widgets/Widget.hpp"

namespace Gecko
{
    class OrdersWidget :
        public Widget
    {
    public:
        struct Order
        {
            std::string name;
        };

	public:
		// From Rml::EventListener.
		void ProcessEvent(Rml::Event& event) override;

    public:
		// From Widget.
        void init_data_bindigs(Rml::Context* context) override;

        void init_events(Rml::ElementDocument* document) override;
        void deinit_events(Rml::ElementDocument* document) override;

    public:
        void update(const std::set<std::string>& orders);

    private:
        Rml::Vector<Order> m_orders;
        std::string m_selected_order;
    };
}
