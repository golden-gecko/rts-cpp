#pragma once

#include "Gecko/UI/Widgets/Widget.hpp"

namespace Gecko
{
    class OrderQueueWidget :
        public Widget
    {
    public:
        struct Order
        {
            int         id;
            std::string name;
            std::string target;
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
        void cancel(Rml::DataModelHandle model, Rml::Event& ev, const Rml::VariantList& arguments);
        void update();

    private:
        Rml::Vector<Order> m_orders;
    };
}
