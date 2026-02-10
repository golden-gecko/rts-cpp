#include "Gecko/UI/Widgets/ObjectsViewer.hpp"

#include "Gecko/Containers/Orders.hpp"
#include "Gecko/Managers/ObjectManager.hpp"
#include "Gecko/Managers/OrderManager.hpp"
#include "Gecko/Objects/Object.hpp"
#include "Gecko/Orders/Order.hpp"

namespace Gecko
{
    void ObjectsViewerWidget::ProcessEvent(Rml::Event& event)
    {
    }

    void ObjectsViewerWidget::init_data_bindigs(Rml::Context* context)
    {
        if (Rml::DataModelConstructor constructor = context->CreateDataModel("objects_viewer"))
        {
            if (Rml::StructHandle handle = constructor.RegisterStruct<Object>())
            {
                handle.RegisterMember("id", &Object::id);
                handle.RegisterMember("name", &Object::name);
                handle.RegisterMember("order_count", &Object::order_count);
                handle.RegisterMember("current_order_name", &Object::current_order_name);

                constructor.RegisterArray<Rml::Vector<Object>>();
                constructor.Bind("objects", &m_objects);
                constructor.BindEventCallback("close", &ObjectsViewerWidget::close, this);

                m_model = constructor.GetModelHandle();
            }
        }
    }

    void ObjectsViewerWidget::init_events(Rml::ElementDocument* document)
    {
        m_document = document;
    }

    void ObjectsViewerWidget::deinit_events(Rml::ElementDocument* document)
    {
    }

    void ObjectsViewerWidget::close(Rml::DataModelHandle model, Rml::Event& ev, const Rml::VariantList& arguments)
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

    void ObjectsViewerWidget::update()
    {
        m_objects.clear();

        for (const auto& [id, object] : ObjectManager::getSingleton())
        {
            std::shared_ptr<Orders> orders = object->get_orders();
            std::string order_name;

            if (orders->size())
            {
                if (OrderPtr order = OrderManager::getSingleton().get(orders->front()))
                {
                    order_name = order_type::to_string(order->get_type());
                }
            }

            m_objects.push_back({ id.get(), object->get_name(), orders->size(), order_name });
        }

        m_model.DirtyVariable("objects");
    }
}
