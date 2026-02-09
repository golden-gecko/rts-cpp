#include "Gecko/UI/Widgets/GameMenu.hpp"

namespace Gecko
{
    void GameMenuWidget::ProcessEvent(Rml::Event& event)
    {
    }

    void GameMenuWidget::init_data_bindigs(Rml::Context* context)
    {
        if (Rml::DataModelConstructor constructor = context->CreateDataModel("game_menu"))
        {
            constructor.BindEventCallback("open", &GameMenuWidget::open, this);

            m_model = constructor.GetModelHandle();
        }
    }

    void GameMenuWidget::init_events(Rml::ElementDocument* document)
    {
        m_document = document;
    }

    void GameMenuWidget::deinit_events(Rml::ElementDocument* document)
    {
    }

    void GameMenuWidget::open(Rml::DataModelHandle model, Rml::Event& ev, const Rml::VariantList& arguments)
    {
        if (m_document)
        {
            std::string id = (arguments.size() == 1 ? arguments[0].Get<std::string>() : "");

            if (Rml::Element* element = m_document->GetElementById(id))
            {
                element->SetClass("hidden", false);
            }
        }
    }
}
