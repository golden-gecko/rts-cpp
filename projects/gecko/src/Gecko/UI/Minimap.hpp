#pragma once

#include "Gecko/Interfaces/Updatable.hpp"

namespace Gecko
{
    class Minimap
    {
    public:
        explicit Minimap();

        virtual ~Minimap();

    public:
        void click(int x, int y);
        void move(int x, int y);
        void zoom_in();
        void zoom_out();

    private:
        Ogre::TexturePtr     m_texture        = nullptr;
        Ogre::RenderTexture* m_render_texture = nullptr;
        Ogre::Viewport*      m_viewport       = nullptr;

        std::string   m_texture_name = "texture_minimap";
        std::uint16_t m_texture_size = 256;

        float m_min_zoom = 1.0f;
        float m_max_zoom = 16.0f;

        float m_scroll_sensivity = 0.005f;
        float m_zoom_sensivity   = 2.0f;
    };
}
