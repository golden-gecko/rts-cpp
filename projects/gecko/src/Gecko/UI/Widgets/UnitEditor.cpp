#include "Gecko/UI/Widgets/UnitEditor.hpp"

#include "Gecko/Cameras/Camera.hpp"
#include "Gecko/Games/Game.hpp"
#include "Gecko/Layers/Layer.hpp"
#include "Gecko/Log.hpp"
#include "Gecko/Maps/Map.hpp"
#include "Gecko/Settings.hpp"

namespace Gecko
{
    void UnitEditorWidget::ProcessEvent(Rml::Event& event)
    {
        static float mouse_start_x = 0.0f;
        static float mouse_start_y = 0.0f;

        switch (event.GetId())
        {
            case Rml::EventId::Mousedown:
            {
                mouse_start_x = event.GetParameter<float>("mouse_x", 0.0f);
                mouse_start_y = event.GetParameter<float>("mouse_y", 0.0f);

                event.StopPropagation();

                break;
            }

            case Rml::EventId::Mousescroll:
            {
                event.GetParameter<float>("wheel_delta_y", 0) > 0.0f ? zoom_in() : zoom_out();
                event.StopPropagation();

                break;
            }

            case Rml::EventId::Mouseup:
            {
                float mouse_x = event.GetParameter<float>("mouse_x", 0.0f);
                float mouse_y = event.GetParameter<float>("mouse_y", 0.0f);

                float diff_x = mouse_start_x - mouse_x;
                float diff_y = mouse_start_y - mouse_y;

                Rml::Element* element = event.GetCurrentElement();

                float width = element->GetClientWidth();
                float height = element->GetClientHeight();

                if (std::fabsf(diff_x) < 10.0f && std::fabsf(diff_y) < 10.0f)
                {
                    float left = element->GetAbsoluteLeft();
                    float top = element->GetAbsoluteTop();

                    if (width != 0.0f && height != 0.0f)
                    {
                        click((mouse_x - left) / width, (mouse_y - top) / height);
                    }
                }
                else
                {
                    if (width != 0.0f && height != 0.0f)
                    {
                        move(diff_x, diff_y);
                    }
                }

                event.StopPropagation();

                break;
            }
        }
    }

    void UnitEditorWidget::init_events(Rml::ElementDocument* document)
    {
        base_type::init_events(document);

        if (document)
        {
            if (Rml::Element* element = document->GetElementById("minimap"))
            {
                element->AddEventListener(Rml::EventId::Mousedown, this);
                element->AddEventListener(Rml::EventId::Mousescroll, this);
                element->AddEventListener(Rml::EventId::Mouseup, this);
            }
        }
    }

    void UnitEditorWidget::deinit_events(Rml::ElementDocument* document)
    {
        base_type::deinit_events(document);

        if (document)
        {
            if (Rml::Element* element = document->GetElementById("minimap"))
            {
                element->RemoveEventListener(Rml::EventId::Mousedown, this);
                element->RemoveEventListener(Rml::EventId::Mousescroll, this);
                element->RemoveEventListener(Rml::EventId::Mouseup, this);
            }
        }
    }

    UnitEditorWidget::UnitEditorWidget()
    {
        m_texture = Ogre::TextureManager::getSingleton().createManual(
            m_texture_name,
            Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
            Ogre::TEX_TYPE_2D,
            m_texture_size, m_texture_size,
            0,
            Ogre::PF_R8G8B8,
            Ogre::TU_RENDERTARGET
        );

        if (MapPtr map = Game::getSingleton().get_active_map())
        {
            if (CameraPtr camera = map->get_camera(Settings::Camera::Minimap))
            {
                if (LayerPtr terrain = map->get_layer(Settings::Layer::Terrain))
                {
                    auto size_x = terrain->get_size() * terrain->get_scale().x;
                    auto size_y = terrain->get_size() * terrain->get_scale().y;
                    auto size_z = terrain->get_size() * terrain->get_scale().z;

                    camera->get_camera()->setOrthoWindow(size_x, size_z);
                    camera->set_position(Ogre::Vector3(size_x / 2.0f, size_y, size_z / 2.0f)); // TODO: Hardcoded.
                    camera->look_at(Ogre::Vector3(size_x / 2.0f, 0.0f, (size_z / 2.0f) - 0.01f)); // TODO: Hardcoded.

                    m_render_texture = m_texture->getBuffer()->getRenderTarget();

                    m_viewport = m_render_texture->addViewport(camera->get_camera());
                    m_viewport->setOverlaysEnabled(false);
                    m_viewport->setShadowsEnabled(false);
                    m_viewport->setSkiesEnabled(false);
                }
            }
        }
    }

    UnitEditorWidget::~UnitEditorWidget()
    {
        Ogre::TextureManager::getSingleton().remove(m_texture);
    }

    void UnitEditorWidget::close(Rml::DataModelHandle model, Rml::Event& ev, const Rml::VariantList& arguments)
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

    void UnitEditorWidget::click(float x, float y)
    {
        L_TRACE << "Minimap::click(" << x << ", " << y << ")";

        if (MapPtr map = Game::getSingleton().get_active_map())
        {
            if (LayerPtr terrain = map->get_layer(Settings::Layer::Terrain))
            {
                if (CameraPtr camera = map->get_camera(Settings::Camera::Main))
                {
                    float position_x = (terrain->get_size() * terrain->get_scale().x) * x;
                    float position_z = (terrain->get_size() * terrain->get_scale().z) * y;

                    camera->set_position(Ogre::Vector3(position_x, camera->get_position().y, position_z));
                }
            }
        }
    }

    void UnitEditorWidget::move(float x, float y)
    {
        L_TRACE << "Minimap::move(" << x << ", " << y << ")";

        if (MapPtr map = Game::getSingleton().get_active_map())
        {
            if (CameraPtr camera = map->get_camera(Settings::Camera::Minimap))
            {
                if (LayerPtr terrain = map->get_layer(Settings::Layer::Terrain))
                {
                    camera->set_position(camera->get_position() + Ogre::Vector3(x * 10.0f, 0.0f, y * 10.0f)); // TODO: Hardcoded.
                }
            }
        }
    }

    void UnitEditorWidget::zoom_in()
    {
        L_TRACE << "Minimap::zoom_in()";

        if (MapPtr map = Game::getSingleton().get_active_map())
        {
            if (CameraPtr camera = map->get_camera(Settings::Camera::Minimap))
            {
                camera->get_camera()->setOrthoWindow(
                    camera->get_camera()->getOrthoWindowWidth() * 0.5f,
                    camera->get_camera()->getOrthoWindowHeight() * 0.5f
                );
            }
        }
    }

    void UnitEditorWidget::zoom_out()
    {
        L_TRACE << "Minimap::zoom_out()";

        if (MapPtr map = Game::getSingleton().get_active_map())
        {
            if (CameraPtr camera = map->get_camera(Settings::Camera::Minimap))
            {
                camera->get_camera()->setOrthoWindow(
                    camera->get_camera()->getOrthoWindowWidth() * 2.0f,
                    camera->get_camera()->getOrthoWindowHeight() * 2.0f
                );
            }
        }
    }
}
