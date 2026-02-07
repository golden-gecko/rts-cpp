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

                m_model = constructor.GetModelHandle();
            }
        }
    }

    void DiplomacyWidget::init_events(Rml::ElementDocument* document)
    {
    }

    void DiplomacyWidget::deinit_events(Rml::ElementDocument* document)
    {
    }
}
