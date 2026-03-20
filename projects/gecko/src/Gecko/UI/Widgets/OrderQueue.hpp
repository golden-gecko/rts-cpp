#pragma once

#include "Gecko/UI/Widgets/Widget.hpp"

namespace Gecko
{
    class OrderQueueWidget :
        public Widget
    {
	// From Widget.
	public:
        void init_data_bindigs(Rml::Context* context) override;

    // OrderQueueWidget.
    public:
        void cancel(Rml::DataModelHandle model, Rml::Event& ev, const Rml::VariantList& arguments);
        void update();

    private:
        struct Order
        {
            int         id;
            std::string name;
            std::string target;
        };

        Rml::Vector<Order> m_orders;
    };
}
