#pragma once

namespace Gecko
{
	class EventListener :
		public Rml::EventListener
	{
	public:
		// From Rml::EventListener.
		void ProcessEvent(Rml::Event& event) override;

		void OnAttach(Rml::Element* element) override;
		void OnDetach(Rml::Element* element) override;

	public:
		EventListener(const Rml::String& value);

	private:
		Rml::Element* get_element(const std::string& element) const;

		void on_close(const std::string& value) const;
		void on_open(const std::string& value) const;
		void on_quit_to_menu() const;
		void on_quit_to_desktop() const;

	private:
		Rml::String m_value;
	};
}
