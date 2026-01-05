#include "Gecko/UI/Preview.hpp"

#include "Gecko/Games/Game.hpp"

namespace Gecko
{
    Preview::Preview()
    {
        /*
        rttTexture = Ogre::TextureManager::getSingleton().createManual(
            "texture_preview",
            Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
            Ogre::TEX_TYPE_2D,
            texture_size, texture_size,
            0,
            Ogre::PF_R8G8B8,
            Ogre::TU_RENDERTARGET
        );

        auto camera = Game::getSingleton().get_camera("Preview")->get_camera();
        auto map = ;

        renderTexture = rttTexture->getBuffer()->getRenderTarget();

        viewport = renderTexture->addViewport(camera);
        viewport->setAutoUpdated(false);
        viewport->setClearEveryFrame(false);
        viewport->setOverlaysEnabled(false);

        renderMaterial =
            Ogre::MaterialManager::getSingleton().create(
                "material_preview",
                Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME
            );

        renderMaterial->getTechnique(0)->getPass(0)->setLightingEnabled(false);

        auto texture_unit_state = renderMaterial->getTechnique(0)->getPass(0)->createTextureUnitState("texture_preview");

        texture_unit_state->setTextureFiltering(
            Ogre::FilterOptions::FO_NONE,
            Ogre::FilterOptions::FO_NONE,
            Ogre::FilterOptions::FO_NONE
        );

        rectangle = new Ogre::Rectangle2D(true);

        float minimap_size = static_cast<float>(size);

        rectangle = new Ogre::Rectangle2D(true);
        rectangle->setBoundingBox(Ogre::AxisAlignedBox::BOX_INFINITE);
        rectangle->setMaterial(renderMaterial);

        // Create scene node.
        scene_node = Game::getSingleton().create_scene_node();
        scene_node->attachObject(rectangle);
        scene_node->setFixedYawAxis(true);

        set_visible(false);
        */
    }

    Preview::~Preview()
    {
        /*
        Game::getSingleton().destroy_scene_node(scene_node);

        delete rectangle;

        Ogre::TextureManager::getSingleton().remove(rttTexture);
        */
    }

    void Preview::update()
    {
        /*
        TODO: Restore and fix.
        auto map = ;
        auto ui = UI::getSingletonPtr();

        // Save state.
        auto grid_visibility = map->is_grid_visible();
        auto ui_visibility = ui->save_visibility();

        // Disable.
        map->show_grid(false);
        ui->set_visible(false);

        // Render.
        viewport->clear();
        viewport->update();

        // Enable.
        map->show_grid(grid_visibility);
        ui->restore_visibility(ui_visibility);
        */
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
