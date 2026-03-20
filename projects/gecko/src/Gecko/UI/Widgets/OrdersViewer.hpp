#pragma once

#include "Gecko/UI/Widgets/Widget.hpp"

namespace Gecko
{
    class OrdersViewerWidget :
        public Widget
    {
	// From Widget.
	public:
        void init_data_bindigs(Rml::Context* context) override;

    // OrdersViewerWidget.
    public:
        void close(Rml::DataModelHandle model, Rml::Event& ev, const Rml::VariantList& arguments);
        void update();

    private:
        struct Order
        {
            int           id;
            std::string   name;
            std::string   sender_name;
            std::string   receiver_name;
            std::uint16_t attempts;
            std::uint16_t max_attempts;
        };

        Rml::Vector<Order> m_orders;
    };
}
