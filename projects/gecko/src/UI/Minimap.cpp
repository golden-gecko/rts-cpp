#include "Minimap.hpp"

#include "Games/Game.hpp"
#include "Log.hpp"

namespace Gecko
{
    Minimap::Minimap()
    {
        /*
        Ogre::TexturePtr rttTexture = Ogre::TextureManager::getSingleton().createManual(
            "texture_minimap",
            Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
            Ogre::TEX_TYPE_2D,
            texture_size, texture_size,
            0,
            Ogre::PF_R8G8B8,
            Ogre::TU_RENDERTARGET
        );

        auto camera = Game::getSingleton().get_camera("Minimap")->get_camera();
        auto map = ;

        camera->setAutoAspectRatio(false);
        camera->setAspectRatio(1.0f);

        if (map->get_terrain_ptr())
        {
            auto size_x = map->get_terrain().get_size() * map->get_terrain().get_scale().x;
            auto size_z = map->get_terrain().get_size() * map->get_terrain().get_scale().z;

            // TODO: Move to settings.
            // TODO: Refactor 320.0f.
            camera->setPosition(size_x / 2.0f, 320.0f, size_z / 2.0f);
            camera->lookAt(size_x / 2.0f, 0.0f, (size_z / 2.0f) - 0.01f);
            camera->setOrthoWindow(size_x, size_z);
        }

        renderTexture = rttTexture->getBuffer()->getRenderTarget();

        viewport = renderTexture->addViewport(camera);
        viewport->setAutoUpdated(false);
        viewport->setClearEveryFrame(false);
        viewport->setOverlaysEnabled(false);
        viewport->setShadowsEnabled(false);
        viewport->setSkiesEnabled(false);

        renderMaterial = 
            Ogre::MaterialManager::getSingleton().create(
            "material_minimap",
            Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME
        );

        renderMaterial->getTechnique(0)->getPass(0)->setLightingEnabled(false);

        auto texture_unit_state = renderMaterial->getTechnique(0)->getPass(0)->createTextureUnitState("texture_minimap");

        texture_unit_state->setTextureFiltering(
            Ogre::FilterOptions::FO_NONE,
            Ogre::FilterOptions::FO_NONE,
            Ogre::FilterOptions::FO_NONE
        );

        rectangle = new Ogre::Rectangle2D(true);

        float minimap_size = static_cast<float>(size);

        auto window = Game::getSingleton().get_window(Settings::Window::MainName);

        float w = window->get_width();
        float h = window->get_height();

        // TODO: Move to settings.
        float l =  (w / 2.0f - minimap_size - 18.0f) / (w / 2.0f);
        float t = -(h / 2.0f - minimap_size - 18.0f) / (h / 2.0f);
        float r =  (w / 2.0f -                18.0f) / (w / 2.0f);
        float b = -(h / 2.0f -                18.0f) / (h / 2.0f);

        rectangle = new Ogre::Rectangle2D(true);
        rectangle->setBoundingBox(Ogre::AxisAlignedBox::BOX_INFINITE);
        rectangle->setCorners(l, t, r, b, false);
        rectangle->setMaterial(renderMaterial);

        // Create scene node.
        scene_node = Game::getSingleton().create_scene_node();
        scene_node->attachObject(rectangle);
        */
    }

    Minimap::~Minimap()
    {
        Game::getSingleton().destroy_scene_node(scene_node);

        delete rectangle;
    }

    void Minimap::update()
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

    void Minimap::click(int x, int y)
    {
        L_TRACE << "Minimap::click(" << x << ", " << y << ")";

        /*
        TODO: Fix.
        auto camera = Game::getSingleton().get_camera(Settings::Camera::MainName)->get_camera();
        auto position = camera->getPosition();

        auto texture_unit_state = renderMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(0);

        auto u_scroll = texture_unit_state->getTextureUScroll();
        auto v_scroll = texture_unit_state->getTextureVScroll();

        auto u_scale = texture_unit_state->getTextureUScale();
        auto v_scale = texture_unit_state->getTextureVScale();

        L_DEBUG << "scroll: " << u_scroll << ":" << v_scroll;
        L_DEBUG << "scale: " << u_scale << ":" << v_scale;

        // TODO: Read from map.
        auto minimap_size = static_cast<float>(size);

        x += minimap_size * u_scroll;
        y += minimap_size * v_scroll;

        if (x >= minimap_size)
        {
            x -= std::floor(x / minimap_size) * minimap_size;
        }

        if (y >= minimap_size)
        {
            y -= std::floor(y / minimap_size) * minimap_size;
        }

        auto map = ;

        auto size_x = map->get_terrain().get_size() * map->get_terrain().get_scale().x;
        auto size_z = map->get_terrain().get_size() * map->get_terrain().get_scale().z;

        position.x = (size_x * static_cast<float>(x) / minimap_size);
        position.z = (size_z * static_cast<float>(y) / minimap_size);

        camera->setPosition(position);
        */
    }

    void Minimap::move(int x, int y)
    {
        L_TRACE << "Minimap::move(" << x << ", " << y << ")";

        /*
        TODO: Fix.
        auto texture_unit_state = renderMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(0);

        auto u_scroll = texture_unit_state->getTextureUScroll();
        auto v_scroll = texture_unit_state->getTextureVScroll();

        auto u_scale = texture_unit_state->getTextureUScale();
        auto v_scale = texture_unit_state->getTextureVScale();

        // TODO: Move to options.
        u_scroll -= static_cast<float>(x) * (scroll_sensivity / u_scale);
        v_scroll -= static_cast<float>(y) * (scroll_sensivity / v_scale);

        if (u_scroll < 0.0f)
        {
            u_scroll = 1.0f + u_scroll;
        }
        else if (u_scroll >= 1.0f)
        {
            u_scroll -= std::floor(u_scroll);
        }

        if (v_scroll < 0.0f)
        {
            v_scroll = 1.0f + v_scroll;
        }
        else if (v_scroll >= 1.0f)
        {
            v_scroll -= std::floor(v_scroll);
        }

        texture_unit_state->setTextureScroll(u_scroll, v_scroll);
        */
    }

    void Minimap::zoom_in()
    {
        L_TRACE << "Minimap::zoom_in()";

        auto texture_unit_state = renderMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(0);

        auto u_scale = texture_unit_state->getTextureUScale();
        auto v_scale = texture_unit_state->getTextureVScale();

        u_scale = std::clamp(u_scale * zoom_sensivity, min_zoom, max_zoom);
        v_scale = std::clamp(v_scale * zoom_sensivity, min_zoom, max_zoom);

        texture_unit_state->setTextureScale(u_scale, v_scale);
    }

    void Minimap::zoom_out()
    {
        L_TRACE << "Minimap::zoom_out()";

        auto texture_unit_state = renderMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(0);

        auto u_scale = texture_unit_state->getTextureUScale();
        auto v_scale = texture_unit_state->getTextureVScale();

        u_scale = std::clamp(u_scale / zoom_sensivity, min_zoom, max_zoom);
        v_scale = std::clamp(v_scale / zoom_sensivity, min_zoom, max_zoom);

        L_DEBUG << "zoom: " << u_scale << ":" << v_scale;

        texture_unit_state->setTextureScale(u_scale, v_scale);
    }
}
