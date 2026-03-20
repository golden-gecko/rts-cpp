#pragma once

#include "Gecko/UI/Widgets/Widget.hpp"

namespace Gecko
{
    class ConsoleWidget :
        public Widget
    {
    private:
        using base_type = Widget;

	// From Rml::EventListener.
	public:
		void ProcessEvent(Rml::Event& event) override;

	// From Widget.
    public:
        void init_data_bindigs(Rml::Context* context) override;

        void init_events(Rml::ElementDocument* document) override;
        void deinit_events(Rml::ElementDocument* document) override;

    // ConsoleWidget.
    private:
        struct Command
        {
            std::string timestamp;
            std::string command;
        };

        std::vector<Command> m_commands;
        std::int16_t         m_selected_command = 0;
    };
}
