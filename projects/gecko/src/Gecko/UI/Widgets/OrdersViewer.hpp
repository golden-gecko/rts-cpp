#pragma once

#include "Gecko/UI/Widgets/Widget.hpp"

namespace Gecko
{
    class OrdersViewerWidget :
        public Widget
    {
    public:
        struct Order
        {
            int           id;
            std::string   name;
            std::string   sender_name;
            std::string   receiver_name;
            std::uint16_t attempts;
            std::uint16_t max_attempts;
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
        void close(Rml::DataModelHandle model, Rml::Event& ev, const Rml::VariantList& arguments);
        void update();

    private:
        Rml::ElementDocument* m_document = nullptr;

        Rml::Vector<Order> m_orders;
    };
}
