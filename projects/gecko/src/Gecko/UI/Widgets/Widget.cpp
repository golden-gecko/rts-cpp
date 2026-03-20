#include "Gecko/UI/Widgets/Widget.hpp"

namespace Gecko
{
    void Widget::ProcessEvent(Rml::Event& event)
    {
    }

    void Widget::init_data_bindigs(Rml::Context* context)
    {
    }

    void Widget::init_events(Rml::ElementDocument* document)
    {
        m_document = document;
    }

    void Widget::deinit_events(Rml::ElementDocument* document)
    {
    }
}
