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
		Rml::String value;
	};
}
