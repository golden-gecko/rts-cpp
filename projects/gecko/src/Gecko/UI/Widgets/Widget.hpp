#pragma once

namespace Gecko
{
    class Widget :
        public Rml::EventListener
    {
    public:
        virtual ~Widget() = default;

    public:
        virtual void init_data_bindigs(Rml::Context* context);

        virtual void init_events(Rml::ElementDocument* document);
        virtual void deinit_events(Rml::ElementDocument* document);

    protected:
        Rml::DataModelHandle m_model;
    };
}
