#include "Gecko/UI/Widgets/MainMenu.hpp"

#include "Gecko/Games/Game.hpp"

namespace Gecko
{
    void MainMenuWidget::ProcessEvent(Rml::Event& event)
    {
    }

    void MainMenuWidget::init_data_bindigs(Rml::Context* context)
    {
        if (Rml::DataModelConstructor constructor = context->CreateDataModel("main_menu"))
        {
            constructor.BindEventCallback("quit", &MainMenuWidget::quit, this);

            m_model = constructor.GetModelHandle();
        }
    }

    void MainMenuWidget::init_events(Rml::ElementDocument* document)
    {
        m_document = document;
    }

    void MainMenuWidget::deinit_events(Rml::ElementDocument* document)
    {
    }

    void MainMenuWidget::quit(Rml::DataModelHandle model, Rml::Event& ev, const Rml::VariantList& arguments)
    {
        Game::getSingleton().quit();
    }
}
