#pragma once

namespace Gecko
{
    class Console :
        public Rml::EventListener
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
        void init_events(Rml::Element* element);
        void deinit_events(Rml::Element* element);

    public:
        const std::vector<Command>& get_commands() const
        {
            return m_commands;
        }

    private:
        std::vector<Command> m_commands;
        std::int16_t m_current_command = 0;
    };
}
