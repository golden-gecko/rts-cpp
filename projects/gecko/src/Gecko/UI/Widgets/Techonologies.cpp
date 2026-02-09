#include "Gecko/UI/Widgets/Techonologies.hpp"

namespace Gecko
{
    void TechonologiesWidget::ProcessEvent(Rml::Event& event)
    {
    }

    void TechonologiesWidget::init_data_bindigs(Rml::Context* context)
    {
        if (Rml::DataModelConstructor constructor = context->CreateDataModel("technologies"))
        {
            if (Rml::StructHandle handle = constructor.RegisterStruct<Technology>())
            {
                handle.RegisterMember("id", &Technology::id);
                handle.RegisterMember("name", &Technology::name);

                constructor.RegisterArray<Rml::Vector<Technology>>();
                constructor.Bind("technologies", &m_technologies);
                constructor.BindEventCallback("close", &TechonologiesWidget::close, this);
                constructor.BindEventCallback("select", &TechonologiesWidget::select, this);

                m_model = constructor.GetModelHandle();
            }
        }
    }

    void TechonologiesWidget::init_events(Rml::ElementDocument* document)
    {
        m_document = document;
    }

    void TechonologiesWidget::deinit_events(Rml::ElementDocument* document)
    {
    }

    void TechonologiesWidget::close(Rml::DataModelHandle model, Rml::Event& ev, const Rml::VariantList& arguments)
    {
        if (m_document)
        {
            std::string id = (arguments.size() == 1 ? arguments[0].Get<std::string>() : "");

            if (Rml::Element* element = m_document->GetElementById(id))
            {
                element->SetClass("hidden", true);
            }
        }
    }

    void TechonologiesWidget::select(Rml::DataModelHandle model, Rml::Event& ev, const Rml::VariantList& arguments)
    {
        m_selected_technology = (arguments.size() == 1 ? arguments[0].Get<std::string>() : "");

        m_model.DirtyVariable("selected_technology");
    }
}
