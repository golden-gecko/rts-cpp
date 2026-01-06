#include "Gecko/UI/EventListenerInstancer.hpp"

#include "Gecko/UI/EventListener.hpp"

namespace Gecko
{
    Rml::EventListener* EventListenerInstancer::InstanceEventListener(const Rml::String& value, Rml::Element* element)
    {
        return new EventListener(value);
    }
}
