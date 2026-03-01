#pragma once

#include "Gecko/Id.hpp"
#include "Gecko/UI/Widgets/Widget.hpp"

namespace Gecko
{
    class LayersWidget :
        public Widget
    {
    public:
        struct Layer
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
        void select(Rml::DataModelHandle model, Rml::Event& ev, const Rml::VariantList& arguments);
        void update();

    private:
        Rml::ElementDocument* m_document = nullptr;

        Rml::Vector<Layer> m_layers;
    };
}
