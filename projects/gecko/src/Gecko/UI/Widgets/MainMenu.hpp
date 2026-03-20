#pragma once

#include "Gecko/UI/Widgets/Widget.hpp"

namespace Gecko
{
    class MainMenuWidget :
        public Widget
    {
	// From Widget.
    public:
        void init_data_bindigs(Rml::Context* context) override;

    // MainMenuWidget.
    public:
        void quit(Rml::DataModelHandle model, Rml::Event& ev, const Rml::VariantList& arguments);
    };
}
