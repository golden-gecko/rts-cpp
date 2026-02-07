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

                m_model = constructor.GetModelHandle();
            }
        }
    }

    void StatisticsWidget::init_events(Rml::ElementDocument* document)
    {
    }

    void StatisticsWidget::deinit_events(Rml::ElementDocument* document)
    {
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
