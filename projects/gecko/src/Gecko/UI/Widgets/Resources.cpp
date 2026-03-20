#include "Gecko/UI/Widgets/Resources.hpp"

#include "Gecko/Containers/Resources.hpp"

namespace Gecko
{
    void ResourcesWidget::init_data_bindigs(Rml::Context* context)
    {
        if (Rml::DataModelConstructor constructor = context->CreateDataModel("resources"))
        {
            if (Rml::StructHandle handle = constructor.RegisterStruct<Resource>())
            {
                handle.RegisterMember("name", &Resource::name);
                handle.RegisterMember("current", &Resource::current);
                handle.RegisterMember("max", &Resource::max);
                handle.RegisterMember("direction", &Resource::direction);
                handle.RegisterMember("ratio", &Resource::ratio);

                constructor.RegisterArray<Rml::Vector<Resource>>();
                constructor.Bind("resources", &m_resources);

                m_model = constructor.GetModelHandle();
            }
        }
    }

    void ResourcesWidget::update(const ResourcesPtr& resources)
    {
        m_resources.clear();

        for (const auto& i : *(resources.get()))
        {
            float ratio = i.second.get_production() - i.second.get_consumption();

            m_resources.push_back({
                i.first,
                i.second.get_current(),
                i.second.get_max(),
                ratio > 0.0f ? "green" : "red",
                ratio
            });
        }

        m_model.DirtyVariable("resources");
    }
}
