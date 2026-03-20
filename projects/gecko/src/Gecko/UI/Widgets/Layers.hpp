#pragma once

#include "Gecko/Id.hpp"
#include "Gecko/UI/Widgets/Widget.hpp"

namespace Gecko
{
    class LayersWidget :
        public Widget
    {
	// From Widget.
    public:
        void init_data_bindigs(Rml::Context* context) override;

    // LayersWidget.
    public:
        void select(Rml::DataModelHandle model, Rml::Event& ev, const Rml::VariantList& arguments);
        void update();

    private:
        struct Layer
        {
            std::string name;
        };

        Rml::Vector<Layer> m_layers;
    };
}
