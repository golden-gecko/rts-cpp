#include "Gecko/UI/UI.hpp"

#include "Gecko/Cameras/ObjectFollowCamera.hpp"
#include "Gecko/Configuration.hpp"
#include "Gecko/Containers/Components.hpp"
#include "Gecko/Containers/Orders.hpp"
#include "Gecko/Containers/Resources.hpp"
#include "Gecko/Containers/Selection.hpp"
#include "Gecko/Exception.hpp"
#include "Gecko/Games/Game.hpp"
#include "Gecko/Input/Input.hpp"
#include "Gecko/Layers/Layer.hpp"
#include "Gecko/Log.hpp"
#include "Gecko/Managers/MapManager.hpp"
#include "Gecko/Managers/ObjectManager.hpp"
#include "Gecko/Managers/OrderManager.hpp"
#include "Gecko/Managers/PlayerManager.hpp"
#include "Gecko/Maps/Map.hpp"
#include "Gecko/Objects/Object.hpp"
#include "Gecko/Players/Player.hpp"
#include "Gecko/Statistics.hpp"
#include "Gecko/System.hpp"
#include "Gecko/Technologies/TechnologyTree.hpp"
#include "Gecko/UI/EventListener.hpp"
#include "Gecko/UI/EventListenerInstancer.hpp"
#include "Gecko/UI/Indicators/Indicator.hpp"
#include "Gecko/UI/RenderInterface.hpp"
#include "Gecko/UI/SystemInterface.hpp"
#include "Gecko/UI/Widgets/Configurations.hpp"
#include "Gecko/UI/Widgets/Console.hpp"
#include "Gecko/UI/Widgets/Cursor.hpp"
#include "Gecko/UI/Widgets/Diplomacy.hpp"
#include "Gecko/UI/Widgets/GameMenu.hpp"
#include "Gecko/UI/Widgets/Info.hpp"
#include "Gecko/UI/Widgets/Layers.hpp"
#include "Gecko/UI/Widgets/Log.hpp"
#include "Gecko/UI/Widgets/Minimap.hpp"
#include "Gecko/UI/Widgets/ObjectsViewer.hpp"
#include "Gecko/UI/Widgets/OrderQueue.hpp"
#include "Gecko/UI/Widgets/Orders.hpp"
#include "Gecko/UI/Widgets/OrdersViewer.hpp"
#include "Gecko/UI/Widgets/Players.hpp"
#include "Gecko/UI/Widgets/Preview.hpp"
#include "Gecko/UI/Widgets/Resources.hpp"
#include "Gecko/UI/Widgets/SelectionBox.hpp"
#include "Gecko/UI/Widgets/Skills.hpp"
#include "Gecko/UI/Widgets/Statistics.hpp"
#include "Gecko/UI/Widgets/Techonologies.hpp"
#include "Gecko/UI/Widgets/UnitEditor.hpp"
#include "Gecko/Utils/Convert.hpp"
#include "Gecko/Utils/String.hpp"
#include "Gecko/Utils/Time.hpp"
#include "Gecko/Utils/Utils.hpp"

Gecko::UI* Ogre::Singleton<Gecko::UI>::msSingleton = nullptr;

namespace Gecko
{
    /*
    TODO: Implement.

    class IndicatorManager :
        public Manager<Indicator, std::string, Id>
    {
    };
    */

    void UI::init()
    {
        Ogre::RenderWindow* render_window = Game::getSingleton().getRenderWindow();

        m_render_interface = std::make_shared<RenderInterface>(render_window->getWidth(), render_window->getHeight());
        m_system_interface = std::make_shared<SystemInterface>();

        Rml::SetRenderInterface(m_render_interface.get());
        Rml::SetSystemInterface(m_system_interface.get());

        if (Rml::Initialise() == false)
        {
            throw Exception("Failed to initialise RmlUi.");
        }

        m_context = Rml::CreateContext("main", Rml::Vector2i(render_window->getWidth(), render_window->getHeight()));

        if (m_context == nullptr)
        {
            throw Exception("Failed to create RmlUi context.");
        }

        init_widgets();
        init_data_bindings();
        init_fonts();
        init_documents();
        init_events();
    }

    void UI::deinit()
    {
        deinit_events();
        deinit_documents();
        deinit_widgets();

        Rml::Shutdown();
    }

    void UI::update(float time)
    {
        if (m_refresh_time.update(time))
        {
            m_refresh_time.reset();

            get_component<DiplomacyWidget>()->update();
            get_component<InfoWidget>()->update(m_hovered_object_id);
            get_component<LayersWidget>()->update();
            get_component<ObjectsViewerWidget>()->update();
            get_component<OrderQueueWidget>()->update();
            get_component<OrdersViewerWidget>()->update();
            get_component<PlayersWidget>()->update();
            get_component<PreviewWidget>()->update();
            get_component<ResourcesWidget>()->update(Game::getSingleton().get_active_player()->get_resources());
            get_component<StatisticsWidget>()->update();
        }
    }

    UI::UI(const ConfigurationPtr& configuration, const ScenePtr& scene) :
        m_configuration(configuration),
        m_scene(scene)
    {
    }

    bool UI::inject_key_press(Rml::Input::KeyIdentifier key)
    {
        // L_TRACE << "UI::inject_key_press(" << key << ")";

        switch (key)
        {
            case Rml::Input::KeyIdentifier::KI_F1:
                m_document->GetElementById("console")->SetClass("hidden", !m_document->GetElementById("console")->IsClassSet("hidden"));
                return true;

            case Rml::Input::KeyIdentifier::KI_F5:
                init_documents();
                init_events();
                return true;

            case Rml::Input::KeyIdentifier::KI_F8:
                Rml::Debugger::SetVisible(!Rml::Debugger::IsVisible());
                return true;
        }

        return m_context->ProcessKeyDown(key, 0);
    }

    bool UI::inject_key_release(Rml::Input::KeyIdentifier key)
    {
        return m_context->ProcessKeyUp(key, 0);
    }

    bool UI::inject_text(unsigned int text)
    {
        return m_context->ProcessTextInput(static_cast<char>(text));
    }

    bool UI::inject_mouse_move(int x, int y, int z)
    {
        if (z == 0)
        {
            return m_context->ProcessMouseMove(x, y, 0);
        }
        else
        {
            return m_context->ProcessMouseWheel(static_cast<float>(z), 0);
        }
    }

    bool UI::inject_mouse_press(int button)
    {
        return m_context->ProcessMouseButtonDown(button, 0);
    }

    bool UI::inject_mouse_release(int button)
    {
        return m_context->ProcessMouseButtonUp(button, 0);
    }

    bool UI::is_mouse_inside(int x, int y)
    {
        Rml::ElementList elements;

        m_document->GetElementsByClassName(elements, "panel");

        for (Rml::Element* element : elements)
        {
            if (element->IsPointWithinElement(Rml::Vector2f(x, y)))
            {
                return true;
            }
        }

        return false;
    }

    void UI::log_error(const std::string& text, const Id& id)
    {
        log_write(text, "error", id);
    }

    void UI::log_info(const std::string& text, const Id& id)
    {
        log_write(text, "info", id);
    }

    void UI::reset()
    {
        reset_configurations();
        reset_orders();
        reset_skills();
    }

    void UI::reset_configurations()
    {
        if (auto configurations = get_component<ConfigurationsWidget>())
        {
            configurations->select("");
        }
    }

    void UI::reset_orders()
    {
        if (auto orders = get_component<OrdersWidget>())
        {
            orders->select("");
        }
    }

    void UI::reset_skills()
    {
        if (auto skills = get_component<SkillsWidget>())
        {
            skills->select("");
        }
    }

    void UI::set_hovered_object_id(const Id& hovered_object_id)
    {
        m_hovered_object_id = hovered_object_id;
    }

    void UI::init_data_bindings()
    {
        for (const auto& i : m_widgets)
        {
            i->init_data_bindigs(m_context);
        }
    }

    void UI::init_documents()
    {
        Rml::Debugger::Shutdown();

        m_context->UnloadAllDocuments();

        m_document = m_context->LoadDocument(m_configuration->get_string("layout"));
        m_document->ReloadStyleSheet();
        m_document->Show();

        Rml::Debugger::Initialise(m_context);
    }

    void UI::init_events()
    {
        for (const auto& i : m_widgets)
        {
            i->init_events(m_document);
        }
    }

    void UI::init_fonts()
    {
        std::set<Rml::String> fonts = m_configuration->get_string_array<std::set<Rml::String>>("fonts");

        for (const Rml::String& font : fonts)
        {
            Rml::LoadFontFace(font, false);
        }
    }

    void UI::init_widgets()
    {
        m_widgets.push_back(std::make_shared<ConfigurationsWidget>());
        m_widgets.push_back(std::make_shared<ConsoleWidget>());
        m_widgets.push_back(std::make_shared<CursorWidget>(m_scene));
        m_widgets.push_back(std::make_shared<DiplomacyWidget>());
        m_widgets.push_back(std::make_shared<InfoWidget>());
        m_widgets.push_back(std::make_shared<GameMenuWidget>());
        m_widgets.push_back(std::make_shared<LayersWidget>());
        m_widgets.push_back(std::make_shared<LogWidget>());
        m_widgets.push_back(std::make_shared<MinimapWidget>());
        m_widgets.push_back(std::make_shared<ObjectsViewerWidget>());
        m_widgets.push_back(std::make_shared<OrderQueueWidget>());
        m_widgets.push_back(std::make_shared<OrdersWidget>());
        m_widgets.push_back(std::make_shared<OrdersViewerWidget>());
        m_widgets.push_back(std::make_shared<PlayersWidget>());
        m_widgets.push_back(std::make_shared<PreviewWidget>());
        m_widgets.push_back(std::make_shared<ResourcesWidget>());
        m_widgets.push_back(std::make_shared<SelectionBoxWidget>(m_scene));
        m_widgets.push_back(std::make_shared<SkillsWidget>());
        m_widgets.push_back(std::make_shared<StatisticsWidget>());
        m_widgets.push_back(std::make_shared<TechonologiesWidget>());
        m_widgets.push_back(std::make_shared<UnitEditorWidget>());
    }

    void UI::deinit_documents()
    {
        m_context->UnloadAllDocuments();
    }

    void UI::deinit_events()
    {
        for (const auto& i : m_widgets)
        {
            i->deinit_events(m_document);
        }
    }

    void UI::deinit_widgets()
    {
        m_widgets.clear();
    }

    void UI::refresh_indicators(const Id& id)
    {
        /*
        m_indicators.clear();

        if (ObjectPtr hovered_object = ObjectManager::getSingleton().get(id))
        {
            int order_number = 1;

            for (const auto& order_id : hovered_object->get_orders()->get_queue())
            {
                OrderPtr order = OrderManager::getSingleton().get(order_id);

                if (order == nullptr)
                {
                    continue;
                }

                std::vector<std::shared_ptr<Indicator>> indicators = order->generate_indicators(order_number);

                if (indicators.size())
                {
                    m_indicators.append_range(indicators);

                    order_number += 1;

                    if (order_number > 3)
                    {
                        break;
                    }
                }
            }
        }
        */
    }

    void UI::log_write(const std::string& message, const std::string& type, const Id& id)
    {
        if (std::shared_ptr<LogWidget> log = get_component<LogWidget>())
        {
            log->write(message, type, id);
        }
    }
}
