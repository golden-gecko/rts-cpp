#include "Gecko/UI/Widgets/Configurations.hpp"

namespace Gecko
{
    void ConfigurationsWidget::ProcessEvent(Rml::Event& event)
    {
    }

    void ConfigurationsWidget::init_data_bindigs(Rml::Context* context)
    {
        if (Rml::DataModelConstructor constructor = context->CreateDataModel("configurations"))
        {
            if (Rml::StructHandle handle = constructor.RegisterStruct<Configuration>())
            {
                handle.RegisterMember("name", &Configuration::name);

                constructor.RegisterArray<Rml::Vector<Configuration>>();
                constructor.Bind("configurations", &m_configurations);
                constructor.Bind("selected_configuration", &m_selected_configuration);
                constructor.BindEventCallback("select", &ConfigurationsWidget::select, this);

                m_model = constructor.GetModelHandle();
            }
        }
    }

    void ConfigurationsWidget::init_events(Rml::ElementDocument* document)
    {
    }

    void ConfigurationsWidget::deinit_events(Rml::ElementDocument* document)
    {
    }

    void ConfigurationsWidget::select(Rml::DataModelHandle model, Rml::Event& ev, const Rml::VariantList& arguments)
    {
        m_selected_configuration = (arguments.size() == 1 ? arguments[0].Get<std::string>() : "");

        m_model.DirtyVariable("selected_configuration");
    }

    void ConfigurationsWidget::update(const std::set<std::string>& configurations)
    {
        m_configurations.clear();

        for (const auto& i : configurations)
        {
            m_configurations.push_back({ i });
        }

        m_model.DirtyVariable("configurations");
    }
}
