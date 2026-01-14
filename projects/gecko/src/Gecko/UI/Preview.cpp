#include "Gecko/UI/Preview.hpp"

#include "Gecko/Cameras/Camera.hpp"
#include "Gecko/Games/Game.hpp"
#include "Gecko/Layers/Layer.hpp"
#include "Gecko/Log.hpp"
#include "Gecko/Managers/MapManager.hpp"
#include "Gecko/Maps/Map.hpp"
#include "Gecko/Settings.hpp"
#include "Gecko/UI/UI.hpp"

namespace Gecko
{
    void Preview::update(float time)
    {
        // TODO: Hardcoded map and camera.
        auto map = MapManager::getSingleton().begin()->second;
        auto camera = map->get_camera("Preview");

        if (camera)
        {
            // TODO: Hardcoded map.
            auto map = MapManager::getSingleton().begin()->second;
            auto ui = UI::getSingletonPtr();

            // Save state.
            // auto grid_visibility = map->is_grid_visible();
            // auto ui_visibility = ui->save_visibility();

            // Disable.
            // map->show_grid(false);
            // ui->set_visible(false);

            // Render.
            // viewport->clear();
            // viewport->update();

            // Enable.
            // map->show_grid(grid_visibility);
            // ui->restore_visibility(ui_visibility);
        }
    }

    Preview::Preview()
    {
        rttTexture = Ogre::TextureManager::getSingleton().createManual(
            "texture_preview",
            Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
            Ogre::TEX_TYPE_2D,
            texture_size, texture_size,
            0,
            Ogre::PF_R8G8B8,
            Ogre::TU_RENDERTARGET
        );

        // TODO: Hardcoded map and camera.
        auto map = MapManager::getSingleton().begin()->second;
        auto camera = map->get_camera("Preview");

        if (camera)
        {
            camera->get_camera()->setAutoAspectRatio(false);
            camera->get_camera()->setAspectRatio(1.0f);

            auto terrain = map->get_layer("Terrain");

            if (terrain)
            {
                auto size_x = terrain->get_size() * terrain->get_scale().x;
                auto size_z = terrain->get_size() * terrain->get_scale().z;

                // TODO: Move to settings.
                // TODO: Refactor 320.0f.
                camera->get_camera_node()->setPosition(Ogre::Vector3(size_x / 2.0f, 320.0f, size_z / 2.0f));
                camera->get_camera_node()->lookAt(Ogre::Vector3(size_x / 2.0f, 0.0f, (size_z / 2.0f) - 0.01f), Ogre::Node::TransformSpace::TS_PARENT);
                camera->get_camera()->setOrthoWindow(size_x, size_z);
            }

            renderTexture = rttTexture->getBuffer()->getRenderTarget();

            viewport = renderTexture->addViewport(camera->get_camera());
            viewport->setOverlaysEnabled(false);
            viewport->setShadowsEnabled(false);
            viewport->setSkiesEnabled(false);
        }
    }

    Preview::~Preview()
    {
        Ogre::TextureManager::getSingleton().remove(rttTexture);
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
