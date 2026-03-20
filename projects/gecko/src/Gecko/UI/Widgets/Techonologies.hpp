#pragma once

#include "Gecko/UI/Widgets/Widget.hpp"

namespace Gecko
{
    class TechonologiesWidget :
        public Widget
    {
	// From Widget.
    public:
        void init_data_bindigs(Rml::Context* context) override;

    // TechonologiesWidget.
    public:
        void close(Rml::DataModelHandle model, Rml::Event& ev, const Rml::VariantList& arguments);
        void select(Rml::DataModelHandle model, Rml::Event& ev, const Rml::VariantList& arguments);

    private:
        struct Technology
        {
            int         id;
            std::string name;
        };

        Rml::Vector<Technology> m_technologies;
        std::string             m_selected_technology;
    };
}
