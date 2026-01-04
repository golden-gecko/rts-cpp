#pragma once

namespace Gecko
{
	class EventInstancer :
		public Rml::EventListenerInstancer
	{
	public:
		// From Rml::EventListenerInstancer.
		Rml::EventListener* InstanceEventListener(const Rml::String& value, Rml::Element * element) override;
	};

	class Event :
		public Rml::EventListener
	{
	public:
		// From Rml::EventListener.
		void ProcessEvent(Rml::Event& event) override;

		void OnAttach(Rml::Element* element) override;
		void OnDetach(Rml::Element* element) override;

	public:
		Event(const Rml::String& value);

	private:
		Rml::String value;
	};
}
