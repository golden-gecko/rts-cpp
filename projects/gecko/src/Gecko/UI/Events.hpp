#pragma once

namespace Gecko
{
    // TODO: Rename.
    class Instancer :
        public Rml::EventListenerInstancer
    {
    public:
        // From Rml::EventListener.
        Rml::EventListener* InstanceEventListener(const Rml::String& value, Rml::Element* element) override;
    };

    // TODO: Rename.
    class Events :
        public Rml::EventListener
    {
    public:
        // From Rml::EventListener.
        void ProcessEvent(Rml::Event& event) override;
        
        void OnAttach(Rml::Element* element) override;
        void OnDetach(Rml::Element* element) override;
    };
}
