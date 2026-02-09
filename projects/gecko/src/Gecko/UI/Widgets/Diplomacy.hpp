#pragma once

#include "Gecko/UI/Widgets/Widget.hpp"

namespace Gecko
{
    class DiplomacyWidget :
        public Widget
    {
    public:
        struct Player
        {
            int         id;
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
        void close(Rml::DataModelHandle model, Rml::Event& ev, const Rml::VariantList& arguments);

    private:
        Rml::ElementDocument* m_document = { nullptr };

        Rml::Vector<Player> m_players;
    };
}
