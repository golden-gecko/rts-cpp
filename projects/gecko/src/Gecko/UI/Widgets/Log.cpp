#include "Gecko/UI/Widgets/Log.hpp"

namespace Gecko
{
    void LogWidget::init_data_bindigs(Rml::Context* context)
    {
        if (Rml::DataModelConstructor constructor = context->CreateDataModel("log"))
        {
            if (Rml::StructHandle handle = constructor.RegisterStruct<Entry>())
            {
                handle.RegisterMember("type", &Entry::type);
                handle.RegisterMember("message", &Entry::message);

                constructor.RegisterArray<Rml::Vector<Entry>>();
                constructor.Bind("entries", &m_entries);

                m_model = constructor.GetModelHandle();
            }
        }
    }

    void LogWidget::write(const std::string& message, const std::string& type, const Id& id)
    {
        m_entries.push_back({ type, message });

        if (m_entries.size() > 3)
        {
            m_entries.erase(m_entries.cbegin(), m_entries.cbegin() + (m_entries.size() - 3));
        }

        m_model.DirtyVariable("entries");
    }
}
