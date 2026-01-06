#pragma once

namespace Gecko
{
	class EventListenerInstancer :
		public Rml::EventListenerInstancer
	{
	public:
		// From Rml::EventListenerInstancer.
		Rml::EventListener* InstanceEventListener(const Rml::String& value, Rml::Element* element) override;
	};
}
