#pragma once

#include "Gecko/UI/Widgets/Widget.hpp"

namespace Gecko
{
    class GameMenuWidget :
        public Widget
    {
	public:
		// From Rml::EventListener.
		void ProcessEvent(Rml::Event& event) override;

    public:
		// From Widget.
        void init_data_bindigs(Rml::Context* context) override;

        void init_events(Rml::ElementDocument* document) override;
        void deinit_events(Rml::ElementDocument* document) override;

    public:
        void open(Rml::DataModelHandle model, Rml::Event& ev, const Rml::VariantList& arguments);

    private:
        Rml::ElementDocument* m_document = nullptr;
    };
}
