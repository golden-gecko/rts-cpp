#include "Gecko/UI/Components/Preview.hpp"

#include "Gecko/Cameras/Camera.hpp"
#include "Gecko/Games/Game.hpp"
#include "Gecko/Layers/Layer.hpp"
#include "Gecko/Log.hpp"
#include "Gecko/Managers/MapManager.hpp"
#include "Gecko/Maps/Map.hpp"
#include "Gecko/Settings.hpp"

namespace Gecko
{
    void Preview::ProcessEvent(Rml::Event& event)
    {
		L_TRACE << "Preview::ProcessEvent()";

        switch (event.GetId())
        {
            case Rml::EventId::Mousescroll:
            {
                event.GetParameter<float>("wheel_delta_y", 0) > 0.0f ? zoom_in() : zoom_out();
                event.StopPropagation();

                break;
            }
        }
    }

    Preview::Preview()
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
            if (CameraPtr camera = map->get_camera(Settings::Camera::Preview))
            {
                m_render_texture = m_texture->getBuffer()->getRenderTarget();

                m_viewport = m_render_texture->addViewport(camera->get_camera());
                m_viewport->setOverlaysEnabled(false);
                m_viewport->setShadowsEnabled(false);
                m_viewport->setSkiesEnabled(false);
            }
        }
    }

    Preview::~Preview()
    {
        Ogre::TextureManager::getSingleton().remove(m_texture);
    }

    void Preview::init_events(Rml::Element* element)
    {
        element->AddEventListener(Rml::EventId::Mousescroll, this);
    }

    void Preview::deinit_events(Rml::Element* element)
    {
        element->RemoveEventListener(Rml::EventId::Mousescroll, this);
    }

    void Preview::zoom_in()
    {
        L_TRACE << "Preview::zoom_in()";

        if (MapPtr map = Game::getSingleton().get_active_map())
        {
            if (std::shared_ptr<ObjectFollowCamera> camera = get_camera())
            {
                camera->set_distance(camera->get_distance() - 10.0f); // TODO: Hardcoded.
            }
        }
    }

    void Preview::zoom_out()
    {
        L_TRACE << "Preview::zoom_out()";

        if (MapPtr map = Game::getSingleton().get_active_map())
        {
            if (std::shared_ptr<ObjectFollowCamera> camera = get_camera())
            {
                camera->set_distance(camera->get_distance() + 10.0f); // TODO: Hardcoded.
            }
        }
    }

    std::shared_ptr<ObjectFollowCamera> Preview::get_camera() const
    {
        if (MapPtr map = Game::getSingleton().get_active_map())
        {
            if (CameraPtr camera = map->get_camera(Settings::Camera::Preview))
            {
                return std::dynamic_pointer_cast<ObjectFollowCamera>(camera);
            }
        }
        
        return nullptr;
    }
}
