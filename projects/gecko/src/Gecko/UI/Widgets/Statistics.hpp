#pragma once

#include "Gecko/UI/Widgets/Widget.hpp"

namespace Gecko
{
    class StatisticsWidget :
        public Widget
    {
	// From Widget.
	public:
        void init_data_bindigs(Rml::Context* context) override;

    // StatisticsWidget.
    public:
        void close(Rml::DataModelHandle model, Rml::Event& ev, const Rml::VariantList& arguments);
        void update();

    private:
        struct Item
        {
            std::string name;
            std::string value;
        };

        Rml::Vector<Item> m_items;
    };
}
