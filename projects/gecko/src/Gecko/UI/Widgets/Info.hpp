#pragma once

#include "Gecko/UI/Widgets/Widget.hpp"

namespace Gecko
{
    class InfoWidget :
        public Widget
    {
    public:
        struct Description
        {
            std::string text;
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
        void set_description(const std::string& text);

    private:
        Description m_description;
    };
}
