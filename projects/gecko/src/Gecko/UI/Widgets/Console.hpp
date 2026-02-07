#pragma once

#include "Gecko/UI/Widgets/Widget.hpp"

namespace Gecko
{
    class ConsoleWidget :
        public Widget
    {
    public:
        struct Command
        {
            std::string timestamp;
            std::string command;
        };

	public:
		// From Rml::EventListener.
		void ProcessEvent(Rml::Event& event) override;

    public:
		// From Widget.
        void init_data_bindigs(Rml::Context* context) override;

        void init_events(Rml::ElementDocument* document) override;
        void deinit_events(Rml::ElementDocument* document) override;

    private:
        std::vector<Command> m_commands;
        std::int16_t m_selected_command = 0;
    };
}
