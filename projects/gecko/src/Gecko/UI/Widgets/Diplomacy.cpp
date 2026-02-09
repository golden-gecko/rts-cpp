#include "Gecko/UI/Widgets/Diplomacy.hpp"

namespace Gecko
{
    void DiplomacyWidget::ProcessEvent(Rml::Event& event)
    {
    }

    void DiplomacyWidget::init_data_bindigs(Rml::Context* context)
    {
        if (Rml::DataModelConstructor constructor = context->CreateDataModel("diplomacy"))
        {
            if (Rml::StructHandle handle = constructor.RegisterStruct<Player>())
            {
                handle.RegisterMember("id", &Player::id);
                handle.RegisterMember("name", &Player::name);

                constructor.RegisterArray<Rml::Vector<Player>>();
                constructor.Bind("players", &m_players);
                constructor.BindEventCallback("close", &DiplomacyWidget::close, this);

                m_model = constructor.GetModelHandle();
            }
        }
    }

    void DiplomacyWidget::init_events(Rml::ElementDocument* document)
    {
        m_document = document;
    }

    void DiplomacyWidget::deinit_events(Rml::ElementDocument* document)
    {
    }

    void DiplomacyWidget::close(Rml::DataModelHandle model, Rml::Event& ev, const Rml::VariantList& arguments)
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
}
