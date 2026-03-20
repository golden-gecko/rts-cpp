#pragma once

#include "Gecko/UI/Widgets/Widget.hpp"

namespace Gecko
{
    class PlayersWidget :
        public Widget
    {
	// From Widget.
	public:
        void init_data_bindigs(Rml::Context* context) override;

    // PlayersWidget.
    public:
        void update();

    private:
        struct Player
        {
            int         id;
            std::string name;
            std::string color;
        };

        Rml::Vector<Player> m_players;
    };
}
