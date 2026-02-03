#include "Gecko/UI/Preview.hpp"

#include "Gecko/Cameras/Camera.hpp"
#include "Gecko/Games/Game.hpp"
#include "Gecko/Layers/Layer.hpp"
#include "Gecko/Log.hpp"
#include "Gecko/Managers/MapManager.hpp"
#include "Gecko/Maps/Map.hpp"
#include "Gecko/Settings.hpp"

namespace Gecko
{
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
            if (CameraPtr camera = map->get_camera("Preview"))
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

    std::shared_ptr<ObjectFollowCamera> Preview::get_camera() const
    {
        // TODO: Hardcoded map and camera.
        auto map = MapManager::getSingleton().begin()->second;
        auto camera = map->get_camera("Preview");

        return std::dynamic_pointer_cast<ObjectFollowCamera>(camera);
    }

    void Preview::set_position(std::uint32_t left, std::uint32_t top, const Id& id)
    {
        /*
        L_TRACE << "Preview::set_position(" << left << ", " << top << ")";

        // Set camera position.
        auto object = ObjectManager::getSingleton().get(id);

        if (object == nullptr)
        {
            L_WARNING << "Object " << id << "does not exist.";

            return;
        }

        auto camera = Game::getSingleton().get_camera("Preview")->get_camera();

        camera->setPosition(object->get_position() + Settings::UI::LookAtObjectOffset);
        camera->lookAt(object->get_position());

        // Set overlay position.
        auto window = Game::getSingleton().get_window(Settings::Window::MainName);

        float w = window->get_width();
        float h = window->get_height();

        auto rect_position = Convert::to_screen_coordinates(left, top, w, h);
        auto rect_size = Convert::to_screen_coordinates(left + size, top + size, w, h);

        auto rectangle_corners = Convert::to_rectangle_coordinates(Ogre::RealRect(
            rect_position.x, rect_position.y,
            rect_size.x, rect_size.y
        ));

        rectangle->setCorners(
            rectangle_corners.left, rectangle_corners.top,
            rectangle_corners.right, rectangle_corners.bottom,
            false
        );
        */
    }

    void Preview::set_visible(bool visible)
    {
        // scene_node->setVisible(visible);
    }
}
