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
		explicit EventListener(const Rml::String& value);

	private:
		Rml::Element* get_element(const std::string& element) const;

	private:
		Rml::String m_value;
	};
}
