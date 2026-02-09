#include "Gecko/UI/Widgets/Statistics.hpp"

#include "Gecko/Statistics.hpp"
#include "Gecko/Utils/Convert.hpp"

namespace Gecko
{
    void StatisticsWidget::ProcessEvent(Rml::Event& event)
    {
    }

    void StatisticsWidget::init_data_bindigs(Rml::Context* context)
    {
        if (Rml::DataModelConstructor constructor = context->CreateDataModel("statistics"))
        {
            if (Rml::StructHandle handle = constructor.RegisterStruct<Item>())
            {
                handle.RegisterMember("name", &Item::name);
                handle.RegisterMember("value", &Item::value);

                constructor.RegisterArray<Rml::Vector<Item>>();
                constructor.Bind("items", &m_items);
                constructor.BindEventCallback("close", &StatisticsWidget::close, this);

                m_model = constructor.GetModelHandle();
            }
        }
    }

    void StatisticsWidget::init_events(Rml::ElementDocument* document)
    {
        m_document = document;
    }

    void StatisticsWidget::deinit_events(Rml::ElementDocument* document)
    {
    }

    void StatisticsWidget::close(Rml::DataModelHandle model, Rml::Event& ev, const Rml::VariantList& arguments)
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

    void StatisticsWidget::update()
    {
        m_items.clear();

        for (const auto& i : Statistics::getSingleton())
        {
            m_items.push_back({ i.first, Utils::Convert::to_string(i.second) });
        }

        m_model.DirtyVariable("items");
    }
}
