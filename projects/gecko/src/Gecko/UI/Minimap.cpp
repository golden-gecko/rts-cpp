#include "Gecko/UI/Minimap.hpp"

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
    void Minimap::update(float time)
    {
        // TODO: Hardcoded map and camera.
        auto map = MapManager::getSingleton().begin()->second;
        auto camera = map->get_camera("Minimap");

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

    Minimap::Minimap()
    {
        rttTexture = Ogre::TextureManager::getSingleton().createManual(
            "texture_minimap",
            Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
            Ogre::TEX_TYPE_2D,
            texture_size, texture_size,
            0,
            Ogre::PF_R8G8B8,
            Ogre::TU_RENDERTARGET
        );

        // TODO: Hardcoded map and camera.
        auto map = MapManager::getSingleton().begin()->second;
        auto camera = map->get_camera("Minimap");

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

    Minimap::~Minimap()
    {
        Ogre::TextureManager::getSingleton().remove(rttTexture);
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

        /*
        TODO: Fix.
        auto texture_unit_state = renderMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(0);

        auto u_scale = texture_unit_state->getTextureUScale();
        auto v_scale = texture_unit_state->getTextureVScale();

        u_scale = std::clamp(u_scale * zoom_sensivity, min_zoom, max_zoom);
        v_scale = std::clamp(v_scale * zoom_sensivity, min_zoom, max_zoom);

        texture_unit_state->setTextureScale(u_scale, v_scale);
        */
    }

    void Minimap::zoom_out()
    {
        L_TRACE << "Minimap::zoom_out()";

        /*
        TODO: Fix.
        auto texture_unit_state = renderMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(0);

        auto u_scale = texture_unit_state->getTextureUScale();
        auto v_scale = texture_unit_state->getTextureVScale();

        u_scale = std::clamp(u_scale / zoom_sensivity, min_zoom, max_zoom);
        v_scale = std::clamp(v_scale / zoom_sensivity, min_zoom, max_zoom);

        L_DEBUG << "zoom: " << u_scale << ":" << v_scale;

        texture_unit_state->setTextureScale(u_scale, v_scale);
        */
    }
}
