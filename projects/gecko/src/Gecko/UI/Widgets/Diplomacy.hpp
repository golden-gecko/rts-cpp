#pragma once

#include "Gecko/UI/Widgets/Widget.hpp"

namespace Gecko
{
    class DiplomacyWidget :
        public Widget
    {
	// From Widget.
    public:
        void init_data_bindigs(Rml::Context* context) override;

    // DiplomacyWidget.
    public:
        void close(Rml::DataModelHandle model, Rml::Event& ev, const Rml::VariantList& arguments);
        void update();

    private:
        struct Player
        {
            std::string              name;
            std::vector<std::string> states;
        };

        Rml::Vector<Player> m_players;
    };
}
