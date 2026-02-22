#include "Gecko/UI/Widgets/Diplomacy.hpp"

#include "Gecko/Diplomacy.hpp"

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
                constructor.RegisterArray<Rml::Vector<Rml::String>>();

                handle.RegisterMember("name", &Player::name);
                handle.RegisterMember("states", &Player::states);

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

    void DiplomacyWidget::update()
    {
        m_players.clear();

        for (const auto& player_a : Diplomacy::getSingleton().get_states())
        {
            Player player;

            player.name = player_a.first;

            for (const auto& player_b : player_a.second)
            {
                player.states.push_back(Diplomacy::to_name(player_b.second));
            }

            m_players.push_back(player);
        }

        m_model.DirtyVariable("players");
    }
}
