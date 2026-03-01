#include "Gecko/UI/Widgets/Layers.hpp"

#include "Gecko/Games/Game.hpp"
#include "Gecko/Layers/Layer.hpp"
#include "Gecko/Maps/Map.hpp"
#include "Gecko/Settings.hpp"

namespace Gecko
{
    void LayersWidget::ProcessEvent(Rml::Event& event)
    {
    }

    void LayersWidget::init_data_bindigs(Rml::Context* context)
    {
        if (Rml::DataModelConstructor constructor = context->CreateDataModel("layers"))
        {
            if (Rml::StructHandle handle = constructor.RegisterStruct<Layer>())
            {
                handle.RegisterMember("name", &Layer::name);

                constructor.RegisterArray<Rml::Vector<Layer>>();
                constructor.Bind("layers", &m_layers);
                constructor.BindEventCallback("select", &LayersWidget::select, this);

                m_model = constructor.GetModelHandle();
            }
        }
    }

    void LayersWidget::init_events(Rml::ElementDocument* document)
    {
        m_document = document;
    }

    void LayersWidget::deinit_events(Rml::ElementDocument* document)
    {
    }

    void LayersWidget::select(Rml::DataModelHandle model, Rml::Event& ev, const Rml::VariantList& arguments)
    {
        if (m_document)
        {
            std::string data_layer_name = (arguments.size() == 1 ? arguments[0].Get<std::string>() : "");

            if (MapPtr map = Game::getSingleton().get_active_map())
            {
                map->show_data_layer(Settings::Layer::Terrain, data_layer_name);
            }
        }
    }

    void LayersWidget::update()
    {
        m_layers.clear();

        if (MapPtr map = Game::getSingleton().get_active_map())
        {
            if (LayerPtr layer = map->get_layer(Settings::Layer::Terrain))
            {
                for (const auto& [name, _] : layer->get_data_layers())
                {
                    m_layers.push_back({ name });
                }
            }
        }

        m_model.DirtyVariable("layers");
    }
}
