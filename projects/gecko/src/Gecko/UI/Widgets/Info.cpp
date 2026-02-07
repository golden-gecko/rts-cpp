#include "Gecko/UI/Widgets/Info.hpp"

namespace Gecko
{
    void InfoWidget::ProcessEvent(Rml::Event& event)
    {
    }

    void InfoWidget::init_data_bindigs(Rml::Context* context)
    {
        if (Rml::DataModelConstructor constructor = context->CreateDataModel("info"))
        {
            if (Rml::StructHandle handle = constructor.RegisterStruct<Description>())
            {
                handle.RegisterMember("text", &Description::text);

                constructor.Bind("description", &m_description);

                m_model = constructor.GetModelHandle();
            }
        }
    }

    void InfoWidget::init_events(Rml::ElementDocument* document)
    {
    }

    void InfoWidget::deinit_events(Rml::ElementDocument* document)
    {
    }

    void InfoWidget::set_description(const std::string& text)
    {
        m_description.text = text;

        m_model.DirtyVariable("description");
    }
}
