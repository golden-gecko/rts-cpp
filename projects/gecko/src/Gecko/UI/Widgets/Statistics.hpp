#pragma once

#include "Gecko/UI/Widgets/Widget.hpp"

namespace Gecko
{
    class StatisticsWidget :
        public Widget
    {
    public:
        struct Item
        {
            std::string name;
            std::string value;
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
        void update();

    private:
        Rml::Vector<Item> m_items;
    };
}
