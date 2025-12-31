#include "Gecko/UI/Events.hpp"

namespace Gecko
{
    Rml::EventListener* Instancer::InstanceEventListener(const Rml::String& value, Rml::Element* element)
    {
        return new Events();
    }

    void Events::ProcessEvent(Rml::Event& event)
    {
        Rml::Input::KeyIdentifier key_identifier;

        switch (event.GetId())
        {
            case Rml::EventId::Keydown:
                key_identifier = (Rml::Input::KeyIdentifier)event.GetParameter<int>("key_identifier", 0);

                if (key_identifier == Rml::Input::KI_ESCAPE)
                {

                }
                break;

            case Rml::EventId::Keyup:
                break;

            case Rml::EventId::Mousedown:
                // Only respond to primary mouse button.
                if (event.GetParameter("button", -1) != 0)
                {
                }
                break;

            case Rml::EventId::Mouseup:
                break;

            case Rml::EventId::Mousemove:
                const Rml::Vector2f mouse_pos(event.GetParameter("mouse_x", 0.f), event.GetParameter("mouse_y", 0.f));
                break;
        }
    }

    void Events::OnAttach(Rml::Element* element)
    {
        Rml::EventListener::OnAttach(element);
    }

    void Events::OnDetach(Rml::Element* element)
    {
        Rml::EventListener::OnDetach(element);

        delete this;
    }
}
