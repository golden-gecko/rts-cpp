#pragma once

#include "Gecko/Id.hpp"
#include "Gecko/UI/Widgets/Widget.hpp"

namespace Gecko
{
    class LogWidget :
        public Widget
    {
    public:
        struct Entry
        {
            std::string type;
            std::string message;
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
        void write(const std::string& message, const std::string& type, const Id& id = Id::Empty);

    private:
        Rml::Vector<Entry> m_entries;
    };
}
