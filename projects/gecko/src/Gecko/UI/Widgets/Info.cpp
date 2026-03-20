#include "Gecko/UI/Widgets/Info.hpp"

#include "Gecko/Containers/Selection.hpp"
#include "Gecko/Games/Game.hpp"
#include "Gecko/Managers/ObjectManager.hpp"
#include "Gecko/Managers/OrderManager.hpp"
#include "Gecko/Managers/PlayerManager.hpp"
#include "Gecko/Objects/Object.hpp"
#include "Gecko/Players/Player.hpp"
#include "Gecko/System.hpp"
#include "Gecko/UI/UI.hpp"
#include "Gecko/UI/Widgets/Cursor.hpp"
#include "Gecko/Utils/Convert.hpp"

namespace Gecko
{
    void InfoWidget::init_data_bindigs(Rml::Context* context)
    {
        if (Rml::DataModelConstructor constructor = context->CreateDataModel("info"))
        {
            if (Rml::StructHandle handle = constructor.RegisterStruct<Description>())
            {
                handle.RegisterMember("text", &Description::text);

                constructor.Bind("description", &m_description);

                m_model = constructor.GetModelHandle();
            }
        }
    }

    void InfoWidget::update(const Id& hovered_object_id)
    {
        // Hovered object info.
        if (ObjectPtr hovered_object = ObjectManager::getSingleton().get(hovered_object_id))
        {
            set_description(hovered_object->get_info());
        }
        // Selected object info.
        else if (Game::getSingleton().get_active_player() && Game::getSingleton().get_active_player()->get_selected()->size())
        {
            Id selected_id = Game::getSingleton().get_active_player()->get_first_selected();

            if (ObjectPtr selected = ObjectManager::getSingleton().get(selected_id))
            {
                set_description(selected->get_info());
            }
        }
        // Global info.
        else
        {
            System::memory_t memory = System::get_memory_usage();
            const Ogre::RenderTarget::FrameStats& window_statistics = Game::getSingleton().getRenderWindow()->getStatistics();

            std::stringstream stream;

            stream << std::fixed << std::setprecision(1);

            stream << "Average FPS: " << Utils::Convert::to_string(window_statistics.avgFPS, 2) << "\n";
            stream << "Frame number: " << Utils::Convert::to_string(Game::getSingleton().get_frame_number()) << "\n";
            stream << "Last FPS: " << Utils::Convert::to_string(window_statistics.lastFPS, 2) << "\n";
            stream << "Triangles: " << window_statistics.triangleCount << "\n\n";

            stream << "Cursor: " << UI::getSingleton().get_component<CursorWidget>()->get_position().to_string() << "\n\n";

            stream << "Objects: " << ObjectManager::getSingleton().size() << "\n";
            stream << "Orders: " << OrderManager::getSingleton().size() << "\n";
            stream << "Players: " << PlayerManager::getSingleton().size() << "\n\n";

            stream << "CPU: " << Utils::Convert::to_string(System::get_cpu_usage()) + "%" << "\n";
            stream << "Memory: " << Utils::Convert::to_string(static_cast<float>(memory.virtual_memory) / 1024.0f / 1024.0f) + " MB";

            set_description(stream.str());
        }
    }

    void InfoWidget::set_description(const std::string& text)
    {
        m_description.text = text;

        m_model.DirtyVariable("description");
    }
}
