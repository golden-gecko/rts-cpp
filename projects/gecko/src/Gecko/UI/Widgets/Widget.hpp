#pragma once

namespace Gecko
{
    class Widget :
        public Rml::EventListener
    {
    // From Rml::EventListener.
	public:
        void ProcessEvent(Rml::Event& event) override;

    // Widget.
    public:
        virtual ~Widget() = default;

    public:
        virtual void init_data_bindigs(Rml::Context* context);

        virtual void init_events(Rml::ElementDocument* document);
        virtual void deinit_events(Rml::ElementDocument* document);

    protected:
        Rml::ElementDocument* m_document = nullptr;
        Rml::DataModelHandle  m_model;
    };
}
